# Instruction costs

|chr                |\*A|\*B|\*C|\*D|\*E|\*F|Additional notes        |
|-------------------|---|---|---|---|---|---|------------------------|
|<code> </code>     |   |   |   |   |   |   |                        |
|`!`                |   |   |   |  4|  2|   |                        |
|`"`                |   |   |   |  n|   |   |n = number of characters|
|`#`                |   |   |   |   |   |   |                        |
|`$`                |   |   |   |  1|   |   |                        |
|`%`                |   |   |   |  1|   |   |                        |
|`&`                | 10|  5|   |  1|   |   |                        |
|`*`                |   |   |   |  1|   |   |                        |
|`+`                |   |   |   |  1|   |   |                        |
|`,`                |  5|  4|   |  1|   |   |                        |
|`-`                |   |   |   |  1|   |   |                        |
|`.`                |  4|  4|   |  1|   |   |                        |
|`/`                |   |   |   |  1|   |   |                        |
|`0` - `9`          |   |   |   |  1|   |   |                        |
|`:`                |   |   |   |  1|   |   |                        |
|`<`                |   |   |   |   |   |  X|                        |
|`>`                |   |   |   |   |   |  X|                        |
|`?`                |   |  1|   | 27|  5|  X|                        |
|`@`                |   |   |  1|  1|   |   |                        |
|`\`                |   |   |   |  1|   |   |                        |
|`^`                |   |   |   |   |   |  X|                        |
|`_`                |   |   |   |  2|  2|  X|                        |
|<code>&#96;</code> |   |   |   |  4|  2|   |                        |
|`v`                |   |   |   |   |   |  X|                        |
|<code>&#124;</code>|   |   |   |  2|  2|  X|                        |
|`~`                |  3|  5|   |  1|   |   |                        |

\*A base costs; instructions
\*B base costs; declarations
\*C base costs; labels
\*D costs each time used; instructions
\*E costs each time used; labels
\*F these will additionally generate either an instruction or a label each time they are visited
