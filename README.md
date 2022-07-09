# ArrayFuck

ArrayFuck is an esoteric programing language.

## Overview

The base of ArrayFuck is a array of integers on indexes from -32768 to 32767. By default, the array is initialized with the value 1. Each index can store values from -32768 to 32767.

There are two possible ways to access the array:

- `[-]`: access the element at the index 0.
- `[<expr>]`: access the element at the index of the expression.

An expression can be either a access to an array or an assignment to an array.

To assign a value to an array, you can use the following syntax:

```ArrayFuck
[<expr>] = <expr>
```

Alternatively, you can use the following syntax:

```ArrayFuck
[<expr>] - <expr>
```

Which will subtract the value of the expression from the element at the index of the expression.

If an assignment is used as an expression, the value of the expression will be returned.

If the square brackets are left empty, it will be used as ASCII io.

### Loops

To make a loop, the colon is used.

```ArrayFuck
<expr>:<assign>
```

The assignment will be executed until the expression is zero.
If used as an expression, the last value assigned will be returned.

### Comments

All characters except for `[`, `]`, `=`, `-` and `:` are ignored.
