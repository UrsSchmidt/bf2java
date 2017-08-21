#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

#define CLASSNAME "Main"
#define STACK 20
#define LOCALS 10

#define in(a,b,c) (((a)<=(b))&&((b)<=(c)))
#define label(x,y,a) (((((y)*(WIDTH))+(x))*10)+(a))

char source[WIDTH][HEIGHT];
bool visited[WIDTH][HEIGHT];
int x = 0, y = 0;
enum { RIGHT, DOWN, LEFT, UP } d = RIGHT;
int locallabel = 0;

/* '"' */
bool stringmode = false;
/* '#' */
int stepsize = 1;

void move() {
    switch (d) {
    case RIGHT:
        x += stepsize;
        x %= WIDTH;
        break;
    case DOWN:
        y += stepsize;
        y %= HEIGHT;
        break;
    case LEFT:
        x -= stepsize;
        if (x < 0) x += WIDTH;
        break;
    case UP:
        y -= stepsize;
        if (y < 0) y += HEIGHT;
        break;
    }
    stepsize = 1;
}

/* mutually recursive */
bool parse_char();

void parse_path() {
    for (;;) {
        if (parse_char()) return;
        move();
    }
}

bool parse_char() {
    const char c = source[x][y];
    if (stringmode) {
        if (c == '"') {
            stringmode = false;
        } else {
            printf("  bipush %d\n", c);
        }
    } else if (in('0', c, '9')) {
        printf("  bipush %d\n", c - '0');
    } else {
        if (c == '<' || c == '>' ||
            c == '?' ||
            c == '^' || c == 'v' ||
            c == '_' || c == '|') {
            /* end path if already visited */
            if (visited[x][y]) {
                printf("  goto LBRANCH%d\n", label(x, y, 0));
                return true;
            }
            /* otherwise generate new label */
            printf("LBRANCH%d:\n", label(x, y, 0));
            visited[x][y] = true;
        }
        switch (c) {
        case ' ': break;
        case '!':
            printf("; '!' begin @%d,%d\n", x, y);
            printf("  ifeq LLOCAL%d\n", locallabel);
            printf("  bipush 0\n");
            printf("  goto LLOCAL%d\n", locallabel + 1);
            printf("LLOCAL%d:\n", locallabel);
            printf("  bipush 1\n");
            printf("LLOCAL%d:\n", locallabel + 1);
            printf("; '!' end @%d,%d\n", x, y);
            locallabel += 2;
            break;
        case '"': stringmode = true; break;
        case '#': stepsize = 2; break;
        case '$': printf("  pop\n"); break;
        case '%': printf("  irem\n"); break;
        case '&': printf("  invokestatic Method %s readInt ()I\n", CLASSNAME); break;
        case '*': printf("  imul\n"); break;
        case '+': printf("  iadd\n"); break;
        case ',': printf("  invokestatic Method %s writeChr (I)V\n", CLASSNAME); break;
        case '-': printf("  isub\n"); break;
        case '.': printf("  invokestatic Method %s writeInt (I)V\n", CLASSNAME); break;
        case '/': printf("  idiv\n"); break;
        case ':': printf("  dup\n"); break;
        case '<': d = LEFT; break;
        case '>': d = RIGHT; break;
        case '?': {
            const int sx = x;
            const int sy = y;
            printf("; '?' begin @%d,%d\n", sx, sy);
            printf("  ldc2_w +4.0\n");
            printf("  invokestatic Method java/lang/Math random ()D\n");
            printf("  dmul\n");
            printf("  d2i\n");
            printf("  dup\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  dup\n");
            printf("  iconst_1\n");
            printf("  isub\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  ifeq LBRANCH%d\n", label(sx, sy, 1));
            printf("  getstatic Field %s rnd I\n", CLASSNAME);
            printf("  dup\n");
            printf("  iconst_1\n");
            printf("  isub\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  ifeq LBRANCH%d\n", label(sx, sy, 2));
            printf("  getstatic Field %s rnd I\n", CLASSNAME);
            printf("  dup\n");
            printf("  iconst_1\n");
            printf("  isub\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  ifeq LBRANCH%d\n", label(sx, sy, 3));
            printf("  goto LBRANCH%d\n", label(sx, sy, 4));
            printf("; '?' middle @%d,%d\n", sx, sy);
            /* right */
            printf("LBRANCH%d:\n", label(sx, sy, 1));
            d = RIGHT;
            move();
            parse_path();
            x = sx;
            y = sy;
            printf("  goto LBRANCH%d\n", label(sx, sy, 5));
            /* down */
            printf("LBRANCH%d:\n", label(sx, sy, 2));
            d = DOWN;
            move();
            parse_path();
            x = sx;
            y = sy;
            printf("  goto LBRANCH%d\n", label(sx, sy, 5));
            /* left */
            printf("LBRANCH%d:\n", label(sx, sy, 3));
            d = LEFT;
            move();
            parse_path();
            x = sx;
            y = sy;
            printf("  goto LBRANCH%d\n", label(sx, sy, 5));
            /* up */
            printf("LBRANCH%d:\n", label(sx, sy, 4));
            d = UP;
            move();
            parse_path();
            x = sx;
            y = sy;
            printf("LBRANCH%d:\n", label(sx, sy, 5));
            printf("; '?' end @%d,%d\n", sx, sy);
        }   break;
        case '@': printf("  goto LHALT\n"); break;
        case'\\': printf("  swap\n"); break;
        case '^': d = UP; break;
        case '`':
            printf("; '`' begin @%d,%d\n", x, y);
            printf("  if_icmpgt LLOCAL%d\n", locallabel);
            printf("  bipush 0\n");
            printf("  goto LLOCAL%d\n", locallabel + 1);
            printf("LLOCAL%d:\n", locallabel);
            printf("  bipush 1\n");
            printf("LLOCAL%d:\n", locallabel + 1);
            printf("; '`' end @%d,%d\n", x, y);
            locallabel += 2;
            break;
     /* case 'g': break; */ /* not supported */
     /* case 'p': break; */ /* not supported */
        case 'v': d = DOWN; break;
        case '~': printf("  invokestatic Method %s readChr ()I\n", CLASSNAME); break;
        case '_':
        case '|': {
            const int sx = x;
            const int sy = y;
            printf("; '%c' begin @%d,%d\n", c, sx, sy);
            printf("  ifne LBRANCH%d\n", label(sx, sy, 1));
            /* right/down (if false) */
            d = (c == '_') ? RIGHT : DOWN;
            move();
            parse_path();
            x = sx;
            y = sy;
            printf("  goto LBRANCH%d\n", label(sx, sy, 2));
            /* left/up (if true) */
            printf("LBRANCH%d:\n", label(sx, sy, 1));
            d = (c == '_') ? LEFT : UP;
            move();
            parse_path();
            x = sx;
            y = sy;
            /* end */
            printf("LBRANCH%d:\n", label(sx, sy, 2));
            printf("; '%c' end @%d,%d\n", c, sx, sy);
        }   break;
        default: printf("; '%c' not supported @%d,%d\n", c, x, y);
        }
        /* end path if branching or halting */
        if (c == '?' || c == '@' ||
            c == '_' || c == '|') return true;
    }
    /* continue path */
    return false;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s src=FILE\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            source[x][y] = ' ';
            visited[x][y] = false;
        }
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Reading file failed\n");
        return EXIT_FAILURE;
    }

    bool using_rint = false; /* '&' */
    bool using_wchr = false; /* ',' */
    bool using_wint = false; /* '.' */
    bool using_rand = false; /* '?' */
    bool using_halt = false; /* '@' */
    bool using_rchr = false; /* '~' */

    /* +2 in case of CRLF */
    char line[WIDTH + 2];
    int y = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int x = 0; x < WIDTH; x++) {
            const char c = line[x];
            if (!in(' ', c, '~')) break;
            source[x][y] = c;
            switch (c) {
            case '&': using_rint = true; break;
            case ',': using_wchr = true; break;
            case '.': using_wint = true; break;
            case '?': using_rand = true; break;
            case '@': using_halt = true; break;
            case '~': using_rchr = true; break;
            }
        }
        y++;
    }
    fclose(file);

    printf(".class public %s\n", CLASSNAME);
    printf(".super java/lang/Object\n");
    if (using_rand)
        printf(".field private static rnd I\n");
    printf("\n");
    if (using_rchr) {
        printf(".method private static readChr : ()I\n");
        printf("  .throws java/io/IOException\n");
        printf("  .limit stack 1\n");
        printf("  .limit locals 0\n");
        printf("  getstatic Field java/lang/System in Ljava/io/InputStream;\n");
        printf("  invokevirtual Method java/io/InputStream read ()I\n");
        printf("  ireturn\n");
        printf(".end method\n");
        printf("\n");
    }
    if (using_rint) {
        printf(".method private static readInt : ()I\n");
        printf("  .throws java/io/IOException\n");
        printf("  .limit stack 5\n");
        printf("  .limit locals 0\n");
        printf("  new java/io/BufferedReader\n");
        printf("  dup\n");
        printf("  new java/io/InputStreamReader\n");
        printf("  dup\n");
        printf("  getstatic Field java/lang/System in Ljava/io/InputStream;\n");
        printf("  invokespecial Method java/io/InputStreamReader <init> (Ljava/io/InputStream;)V\n");
        printf("  invokespecial Method java/io/BufferedReader <init> (Ljava/io/Reader;)V\n");
        printf("  invokevirtual Method java/io/BufferedReader readLine ()Ljava/lang/String;\n");
        printf("  invokestatic Method java/lang/Integer parseInt (Ljava/lang/String;)I\n");
        printf("  ireturn\n");
        printf(".end method\n");
        printf("\n");
    }
    if (using_wchr) {
        printf(".method private static writeChr : (I)V\n");
        printf("  .limit stack 2\n");
        printf("  .limit locals 1\n");
        printf("  getstatic Field java/lang/System out Ljava/io/PrintStream;\n");
        printf("  iload_0\n");
        printf("  i2c\n");
        printf("  invokevirtual Method java/io/PrintStream print (C)V\n");
        printf("  return\n");
        printf(".end method\n");
        printf("\n");
    }
    if (using_wint) {
        printf(".method private static writeInt : (I)V\n");
        printf("  .limit stack 2\n");
        printf("  .limit locals 1\n");
        printf("  getstatic Field java/lang/System out Ljava/io/PrintStream;\n");
        printf("  iload_0\n");
        printf("  invokevirtual Method java/io/PrintStream print (I)V\n");
        printf("  return\n");
        printf(".end method\n");
        printf("\n");
    }
    printf(".method public static main : ([Ljava/lang/String;)V\n");
    if (using_rchr || using_rint)
        printf("  .throws java/io/IOException\n");
    printf("  .limit stack %d\n", STACK);
    printf("  .limit locals %d\n", LOCALS);
    parse_path();
    if (using_halt)
        printf("LHALT:\n");
    printf("  return\n");
    printf(".end method\n");

    return EXIT_SUCCESS;
}
