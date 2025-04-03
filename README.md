# 🐚 Minishell – A Tiny Bash Clone
## Made by [@Anya Zinchenko](https://github.com/greyear) and [@Seela Salorinta](https://github.com/SeelaSalorinta)

Minishell is our very own small-scale Unix shell, built entirely from scratch.
Over the course of two intense months, we poured our time, energy, and
countless discussions into bringing this project to life. It was a true test of
our problem-solving skills, attention to detail, and ability to push each other
forward.

This project was more than just writing a shell—it was about learning to think
like one. We dissected how command execution works, how processes communicate,
and how to manage an ever-changing environment. Through long debugging
sessions, design choices, and moments of triumph, we kept each other motivated
and on pace, making this one of the most rewarding projects we've tackled.

We’re super proud to say we got 101/100 points! ✅

 ⚙️ How to Run It

 1️⃣ Clone the repository
```sh
git clone https://github.com/greyear/minishell && cd minishell
```

 2️⃣ Compile the project
```sh
make
```

 3️⃣ Launch the shell
```sh
./minishell
```

 🔍 Leak Checking
To check for memory leaks using Valgrind (with a suppression file to ignore readline-related leaks), run:
```sh
valgrind --leak-check=full --show-leak-kinds=all --suppressions=val.supp ./minishell
```

### ✨ Features

- **Fully interactive shell** that displays a prompt and executes commands
- **Command execution** using absolute/relative paths or searching in `$PATH`
- **Working command history**

 #### ✅ Redirections & Pipes:
  - `<` Input redirection
  - `>` Output redirection
  - `<<` Here-document (read until delimiter)
  - `>>` Append mode
  - `|` Piping between commands

- **Environment variable expansion** (`$VARIABLE`)
- **Special variable support** (`$?` for the last exit status)

##### ✅ Signal handling:
  - `Ctrl+C` interrupts the current command and shows a new prompt
  - `Ctrl+D` exits the shell
  - `Ctrl+\` does nothing (just like Bash)

#### ✅ Built-in commands:
  - `echo` (with `-n` option)
  - `cd` (relative/absolute paths)
  - `pwd` (print working directory)
  - `export` (set environment variables)
  - `unset` (remove environment variables)
  - `env` (print environment variables)
  - `exit` (close the shell)

💡 Two months of teamwork, debugging, and late-night coding sessions paid off. We pushed ourselves, learned so much, and had a great time working together! 💪🎉 

## Moving on to next coding adventures ! 🏆
