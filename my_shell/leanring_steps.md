# Build Your Own Shell in C: Learning Plan

A guided exercise in how Unix shells work. Each milestone lists what to read, what to decide, and how to test. No solution code is included on purpose.

**Compile with warnings on, always:**

```
gcc -Wall -Wextra -o mysh mysh.c
```

## The core idea

A shell is a loop:

1. Print a prompt
2. Read a line
3. Parse it into a command and arguments
4. Run it
5. Repeat

Running a program uses three system calls: `fork()` (copy the process), `execvp()` (replace the copy with a new program), and `waitpid()` (parent waits for the child).

---

## Progress

| # | Milestone | Status |
|---|-----------|--------|
| 1 | Prompt and read loop | Done |
| 2 | Split line into words | Done (pending cleanup) |
| 3 | Run a program (fork/exec/wait) | Next |
| 4 | Builtins (`cd`, `exit`) | Not started |
| 5 | Redirection | Not started |
| 6 | Pipes | Not started |
| 7 | Background jobs | Not started |
| 8 | Signal handling | Not started |
| 9 | Quoting and escaping | Not started |
| 10 | Environment variables, globbing, scripting | Stretch |

---

## Milestone 1: Prompt and read loop (done)

**Goal:** a single loop that prints a prompt, reads a line, and reacts to it.

**Concepts learned**
- `getline()` allocates and grows the buffer for you; it keeps the trailing `\n`
- Returns `-1` on EOF (Ctrl-D), which is a normal exit, not an error
- stdout is buffered; `fflush(stdout)` forces a prompt with no newline to appear
- Trim the newline in place (`strcspn`) instead of copying
- `free(line)` on every exit path
- Ctrl-C is a signal, not the characters `^C`

**Self-checks**
- A word, a blank line, Ctrl-D on an empty prompt, Ctrl-D after typing text
- `./mysh | cat` to see the buffering behavior

---

## Milestone 2: Split the line into words (done, pending cleanup)

**Goal:** turn `"ls -l /tmp"` into an array of strings ending in `NULL`.

**Concepts learned**
- `strtok` writes `\0` over delimiters, so it modifies the original line
- First call takes the string, later calls take `NULL`; the second argument is the **set of delimiters**
- The tokens array holds **pointers into `line`**, not copies
- `execvp` needs the trailing `NULL` because it has no other way to know the array length
- Loop shape: hold a token, check it is real, store it, fetch the next
- Leave one slot for the `NULL`: loop condition is `num_tokens < MAX_TOKENS - 1`

**Cleanup checklist**
- [ ] `#define MAX_TOKENS 1024` (not `const int`, which makes a variable-length array)
- [ ] Do not name it `BUFSIZ`; that clashes with `<stdio.h>`
- [ ] `continue` when `num_tokens == 0` (blank line)
- [ ] Remove the debug prints
- [ ] `const char *` for the delimiter string
- [ ] Remove the double semicolon

**Self-checks**
- `ls -l /tmp`
- Extra leading, trailing, and repeated spaces
- A tab between words
- Only spaces
- With the limit temporarily at 3, typing `a b c d e` gives exactly two words

---

## Milestone 3: Run a program (next)

**Read first:** `man fork`, `man execvp`, `man waitpid`. Add `<unistd.h>` and `<sys/wait.h>`.

**Questions to answer before writing code**
1. What are the three return values of `fork()` and what does each mean?
2. Which process calls `execvp`? What are its two arguments, and why is `tokens` already the right shape?
3. If `execvp` succeeds, what happens to the code after it? If it fails, what should the child print, and what must it do next? (A failed child that falls back into the shell loop gives you two shells.)
4. What does the parent do after forking, and what goes wrong if it skips that?

**Self-checks**
- `ls`
- `ls -l /tmp`
- `echo hello world`
- `sleep 3` (the prompt should wait)
- `asdfgh` (an error message, then exactly one prompt)

---

## Milestone 4: Builtins

**The puzzle:** type `cd /tmp` into your shell after milestone 3. It runs, but nothing changes. Work out why before reading on. Hint: which process's working directory changes?

**Tasks**
- Check `tokens[0]` against `exit` and `cd` **before** forking
- Read `man chdir`. What should `cd` with no argument do?
- Print an error with `perror` if `chdir` fails
- Move the `exit` check onto `tokens[0]` so `exit` with trailing spaces works

**Self-checks**
- `cd /tmp` then `pwd`
- `cd` with no argument, `cd /nonexistent`
- `exit` with extra whitespace

---

## Milestone 5: Output and input redirection

**Key idea:** file descriptors survive `exec`.

**Read:** `man open`, `man dup2`, `man 2 close`

**Tasks**
- Scan the tokens for `>`, find the filename after it, and remove both from the argument list
- In the **child**, before `execvp`: open the file, `dup2` it onto stdout, close the extra descriptor
- Then extend to `>>` (append) and `<` (input)

**Questions**
- Why must this happen in the child and not the parent?
- What flags does `open` need to create or truncate a file, and what permissions?
- What should happen if `>` is the last token with no filename?

**Self-checks**
- `echo hi > out.txt` then `cat out.txt`
- `ls > a.txt` run twice (truncate) vs `ls >> a.txt` (append)
- `sort < unsorted.txt`

---

## Milestone 6: Pipes

**Read:** `man pipe`

**Tasks**
- Split the token list at `|` into two commands
- Create a pipe, fork twice, wire the first child's stdout to the write end and the second child's stdin to the read end
- Close **every** unused pipe end in every process
- Parent waits for both children

**Questions**
- What happens to the reader if a write end is left open somewhere? (This is the classic "hangs forever" bug.)
- How would you generalize from two commands to N?

**Self-checks**
- `ls | wc -l`
- `ls -l | grep foo`
- `cat file | sort | uniq` (once generalized)

---

## Milestone 7: Background jobs

**Tasks**
- Detect a trailing `&` and strip it
- Fork and exec as usual, but do not wait immediately
- Reap finished children later so they do not stay as zombies (`waitpid` with `WNOHANG`)

**Questions**
- Where in your loop is a good place to reap finished children?
- What does a zombie look like in `ps`?

**Self-checks**
- `sleep 10 &` returns the prompt immediately
- Check `ps` after it finishes: no zombie left

---

## Milestone 8: Signal handling

**Read:** `man signal` or `man sigaction`, and `man setpgid`

**Goal:** Ctrl-C should kill the running child but not your shell.

**Tasks**
- Make the shell ignore (or handle) `SIGINT`
- Make sure children get the default behavior back before `exec`
- Think about process groups and why real shells put each job in its own group

**Self-checks**
- Run `sleep 100`, press Ctrl-C: the sleep dies, the shell survives
- Press Ctrl-C at an empty prompt: the shell survives

---

## Milestone 9: Quoting and escaping

**Goal:** `echo "hello world"` passes one argument, not two.

**Why it is hard:** `strtok` cannot do this. You need a real tokenizer that walks the line character by character and tracks whether it is inside quotes.

**Tasks**
- Write a small state machine: normal, in single quotes, in double quotes
- Decide what backslash does
- Decide what to do about an unterminated quote

---

## Milestone 10: Stretch goals

- Environment variables: `$HOME`, `export` (`getenv`, `setenv`)
- Globbing: `*.txt` (`man glob`)
- Command history and line editing (look at the readline library)
- Tab completion
- `&&`, `||`, `;`
- Scripting: `if`, `while`, functions. This is where a proper lexer and grammar become necessary.

---

## Habits to keep

- Compile with `-Wall -Wextra` every time
- Test the empty case, the too-long case, and the error case, not just the happy path
- Free what you allocate, on every exit path
- Flush stdout before you `fork`
- Read the man page before writing the call
- When a function seems to "work by accident", find out why. That is usually a bug waiting to happen.

## References

I cannot verify links from here, so double-check these yourself:

- Stephen Brennan's "Write a Shell in C" blog post
- *Operating Systems: Three Easy Pieces* (the process API chapter covers fork/exec/wait)
- The man pages: `man 2 fork`, `man 3 exec`, `man 2 waitpid`, `man 2 pipe`, `man 2 dup2`