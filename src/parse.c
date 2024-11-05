#include "parse.h"
#include "map.h"
#include "util.h"
#include "textures.h"
#include <stdbool.h>

struct {
        int txt_index;
        double txt_scale;
        double merge_tol;
} ctl_state;

static void ctl_state_reset()
{
        ctl_state.txt_scale = 1.0;
        ctl_state.merge_tol = 0.1;
        ctl_state.txt_index = PLASTER_INDEX;
}

static void ctl_seq_reset(struct ctl_seq *seq)
{
        seq->param_count = 0;
        memset(seq->id, 0, CTL_TOKEN_LENGTH);
        memset(seq->params, 0, CTL_TOKEN_LENGTH * CTL_SEQ_PARAM_COUNT);
}

#define IS_SPACE(c) (c == ' ' || c == '\t')
#define IS_ENDL(c) (c == '\n')

unsigned long ctl_seq_parse(const char *str, unsigned long length, unsigned long off, struct ctl_seq *seq, _Bool *status)
{
        ctl_seq_reset(seq);

        enum {
                STATE_WORD,
                STATE_PARAM
        } state = STATE_WORD;

        char buffer[CTL_TOKEN_LENGTH] = {0};
        unsigned int buffer_size = 0;

        unsigned long i = off;
        char c;

        _Bool force_break = false;

        for (; i < length; i++) {
                c = str[i];

                if (IS_ENDL(c)) {
                        // We don't care about newlines as long as we didn't start parsing a seq yet
                        if (state == STATE_WORD)
                                continue;

                        if (!buffer_size)
                                break;

                        force_break = true;
                        goto submit_buffer;
                }

                if (IS_SPACE(c)) {
                        if (!buffer_size)
                                continue;

                        goto submit_buffer;
                }

                if (buffer_size + 1 >= CTL_TOKEN_LENGTH) {
                        force_break = true;
                        goto submit_buffer;
                }

                buffer[buffer_size++] = c;

                if (i + 1 < length)
                        continue;

                submit_buffer:
                strcpy(state == STATE_WORD ? seq->id : seq->params[seq->param_count++], buffer);
                state = STATE_PARAM;
                memset(buffer, 0, CTL_TOKEN_LENGTH);
                buffer_size = 0;

                if (force_break)
                        break;
        }

        if (IS_ENDL(c) && i + 1 < length)
                i++;

        if (state == STATE_WORD)
                *status = false;
        else
                *status = true;

        return i;
}

#undef IS_SPACE
#undef IS_ENDL

// Either return a pointer to a new vertex [x, y] or find an existing one at the given location (or inside a given merge_tol)
static struct vector *map_vertex(double x, double y, struct map *map)
{
        struct vector new = vec(x, y);

        for (unsigned long i = 0; i < map->vert_count; i++) {
                struct vector *v = &map->vertices[i];
                if (vector_distance(&new, v) <= ctl_state.merge_tol)
                        return v;
        }

        map->vertices[map->vert_count] = new;
        return &map->vertices[map->vert_count++];
}

static void map_wall(struct vector *a, struct vector *b, struct map *map)
{
        map->segments[map->seg_count++] = (struct segment) {
                .a = a,
                .b = b,
                .txt_scale = ctl_state.txt_scale,
                .txt = ctl_state.txt_index
        };
}

#define HANDLE(str, blk) if (strcmp(seq->id, str) == 0) { blk return true; }

#define CT(expr) if (seq->param_count != expr) {                                                                                \
                        printf("Required parameter count %lu not met with %lu.\n", (unsigned long) expr, seq->param_count);     \
                        return false;                                                                                           \
                 }

#define D(id, i)                                                        \
        if (seq->param_count <= i) return false;                        \
        double id;                                                      \
        if (!str_conv_d(seq->params[i], &id)) {                         \
                printf("Not a double: \"%s\"\n", seq->params[i]);       \
                return false;                                           \
        }

#define S(id, i) char *id = seq->params[i];

#define ASSERT(expr, ...) if (!(expr)) { printf(__VA_ARGS__); return false; }

#define CTL_SEQ seq

static _Bool ctl_seq_execute(struct ctl_seq *CTL_SEQ, struct map *map)
{
        HANDLE("WALL", {
                CT(4)
                D(x1, 0)
                D(y1, 1)
                D(x2, 2)
                D(y2, 3)

                struct vector *vert_a = map_vertex(x1, y1, map);
                struct vector *vert_b = map_vertex(x2, y2, map);

                map_wall(vert_a, vert_b, map);
        })

        HANDLE("MERGE", {
                CT(1)
                D(tolerance, 0)

                ASSERT(tolerance >= 0.0, "The merge tolerance scale is expected to be greater or equal to 0, got %f.\n",
                       tolerance)

                ctl_state.merge_tol = tolerance;
        })

        HANDLE("SCALE", {
                CT(1)
                D(scale, 0)

                ASSERT(scale > 0.0, "The texture scale is expected to be greater than 0, got %f.\n", scale)

                ctl_state.txt_scale = scale;
        })

        HANDLE("TXT", {
                CT(1)
                S(texture, 0)

                int txt_ix = find_texture(texture);

                ASSERT(txt_ix != -1, "Undefined texture '%s'.\n", texture)

                ctl_state.txt_index = txt_ix;
        })

        printf("Unknown control word \"%s\"\n", CTL_SEQ->id);
        return false;
}

static _Bool ctl_seq_parse_exec_all(const char *str, struct map *map)
{
        ctl_state_reset();

        unsigned long len = strlen(str);
        unsigned long off = 0;
        unsigned long seq_count = 0;

        _Bool success = false;

        struct ctl_seq seq;

        parse_seq:
        off = ctl_seq_parse(str, len, off, &seq, &success);

        if (off < len && success) {
                if (!ctl_seq_execute(&seq, map)) {
                        printf("Could not build map: Failed on control sequence \"%s\".\n", seq.id);
                        return false;
                }

                goto parse_seq;
        }

        if (!success) {
                printf("Could not parse control sequence.\n");
                return false;
        }

        return true;
}

#undef HANDLE
#undef CT
#undef D
#undef S
#undef CTL_SEQ
#undef ASSERT

_Bool map_parse(const char *str, struct map *ptr)
{
        ptr->seg_count = 0;
        ptr->vert_count = 0;

        return ctl_seq_parse_exec_all(str, ptr);
}
