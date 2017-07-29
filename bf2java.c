#include <stdbool.h>
#include <stdio.h>

#define WIDTH  80
#define HEIGHT 25

#define CLASSNAME "Main"
#define STACK  20
#define LOCALS 10

#define label(x,y,a) (((((y)*(WIDTH))+(x))*10)+(a))

char source[WIDTH][HEIGHT];
bool visited[WIDTH][HEIGHT];
int x = 0, y = 0;
enum { RIGHT, DOWN, LEFT, UP } d = RIGHT;

/* '"' */
bool stringmode = false;
/* '#' */
int stepsize = 1;
/* '&' */
bool readint = false;
/* ',' */
bool writechr = false;
/* '.' */
bool writeint = false;
/* '?' */
bool random = false;
/* '~' */
bool readchr = false;

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

bool parseChar();

void parsePath() {
    while (true) {
        if (parseChar()) return;
        move();
    }
}

bool parseChar() {
    const char c = source[x][y];
    if (stringmode) {
        if (c == '"') {
            stringmode = false;
        } else {
            printf("  bipush %d\n", c);
        }
    } else if (('0' <= c) && (c <= '9')) {
        printf("  bipush %d\n", c - '0');
    } else {
        if ((c == '<') || (c == '>') ||
            (c == '?') ||
            (c == '^') || (c == 'v') ||
            (c == '_') || (c == '|')) {
            /* end path if already visited */
            if (visited[x][y]) {
                printf("  goto L%d\n", label(x, y, 0));
                return true;
            }
            /* otherwise generate new label */
            printf("L%d:\n", label(x, y, 0));
            visited[x][y] = true;
        }
        switch (c) {
        case ' ': break;
        case '!': {
            printf("; begin '!' @%d,%d\n", x, y);
            printf("  ifeq L%d\n", label(x, y, 1));
            printf("  bipush 0\n");
            printf("  goto L%d\n", label(x, y, 2));
            printf("L%d:\n", label(x, y, 1));
            printf("  bipush 1\n");
            printf("L%d:\n", label(x, y, 2));
            printf("; end '!' @%d,%d\n", x, y);
        }   break;
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
            printf("; begin '?' @%d,%d\n", x, y);
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
            printf("  ifeq L%d\n", label(x, y, 1));
            printf("  getstatic Field %s rnd I\n", CLASSNAME);
            printf("  dup\n");
            printf("  iconst_1\n");
            printf("  isub\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  ifeq L%d\n", label(x, y, 2));
            printf("  getstatic Field %s rnd I\n", CLASSNAME);
            printf("  dup\n");
            printf("  iconst_1\n");
            printf("  isub\n");
            printf("  putstatic Field %s rnd I\n", CLASSNAME);
            printf("  ifeq L%d\n", label(x, y, 3));
            printf("  goto L%d\n", label(x, y, 4));
            printf("; middle '?' @%d,%d\n", x, y);
            const int sx = x;
            const int sy = y;
            /* right */
            printf("L%d:\n", label(sx, sy, 1));
            d = RIGHT;
            move();
            parsePath();
            x = sx;
            y = sy;
            printf("  goto L%d\n", label(sx, sy, 5));
            /* down */
            printf("L%d:\n", label(sx, sy, 2));
            d = DOWN;
            move();
            parsePath();
            x = sx;
            y = sy;
            printf("  goto L%d\n", label(sx, sy, 5));
            /* left */
            printf("L%d:\n", label(sx, sy, 3));
            d = LEFT;
            move();
            parsePath();
            x = sx;
            y = sy;
            printf("  goto L%d\n", label(sx, sy, 5));
            /* up */
            printf("L%d:\n", label(sx, sy, 4));
            d = UP;
            move();
            parsePath();
            x = sx;
            y = sy;
            printf("L%d:\n", label(sx, sy, 5));
            printf("; end '?' @%d,%d\n", sx, sy);
        }   break;
        case '@': printf("  goto LHALT\n"); break;
        case'\\': printf("  swap\n"); break;
        case '^': d = UP; break;
        case '`': {
            printf("; begin '`' @%d,%d\n", x, y);
            printf("  if_icmpgt L%d\n", label(x, y, 1));
            printf("  bipush 0\n");
            printf("  goto L%d\n", label(x, y, 2));
            printf("L%d:\n", label(x, y, 1));
            printf("  bipush 1\n");
            printf("L%d:\n", label(x, y, 2));
            printf("; end '`' @%d,%d\n", x, y);
        }   break;
//      case 'g': break; /* not supported */
//      case 'p': break; /* not supported */
        case 'v': d = DOWN; break;
        case '~': printf("  invokestatic Method %s readChr ()I\n", CLASSNAME); break;
        case '_':
        case '|': {
            printf("; begin '%c' @%d,%d\n", c, x, y);
            printf("  ifne L%d\n", label(x, y, 1));
            const int sx = x;
            const int sy = y;
            /* right/down (false) */
            d = (c == '_') ? RIGHT : DOWN;
            move();
            parsePath();
            x = sx;
            y = sy;
            printf("  goto L%d\n", label(sx, sy, 2));
            /* left/up (true) */
            printf("L%d:\n", label(sx, sy, 1));
            d = (c == '_') ? LEFT : UP;
            move();
            parsePath();
            x = sx;
            y = sy;
            /* end */
            printf("L%d:\n", label(sx, sy, 2));
            printf("; end '%c' @%d,%d\n", c, sx, sy);
        }   break;
        default:  printf("  ; not supported command '%c' @%d,%d\n", c, x, y); break;
        }
        /* end this path if branching */
        if ((c == '?') || (c == '@') ||
            (c == '_') || (c == '|')) return true;
    }
    /* continue this path */
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <src>\n", argv[0]);
        return 0;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            source[x][y] = ' ';
            visited[x][y] = false;
        }
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("Error reading file\n");
        return 0;
    }
    /* +2 in case of CRLF */
    char line[WIDTH + 2];
    int y = 0;
    while (fgets(line, sizeof(line), f)) {
        for (int x = 0; x < WIDTH; x++) {
            const char c = line[x];
            if (!((' ' <= c) && (c <= '~'))) break;
            source[x][y] = c;
            if (c == '&') readint = true;
            if (c == ',') writechr = true;
            if (c == '.') writeint = true;
            if (c == '?') random = true;
            if (c == '~') readchr = true;
        }
        y++;
    }
    fclose(f);

    printf(".class public %s\n", CLASSNAME);
    printf(".super java/lang/Object\n");
    if (random) {
        printf(".field private static rnd I\n");
    }
    printf("\n");
    if (readchr) {
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
    if (readint) {
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
    if (writechr) {
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
    if (writeint) {
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
    if (readchr || readint) {
        printf("  .throws java/io/IOException\n");
    }
    printf("  .limit stack %d\n", STACK);
    printf("  .limit locals %d\n", LOCALS);
    parsePath();
    printf("LHALT:\n");
    printf("  return\n");
    printf(".end method\n");
    return 0;
}
