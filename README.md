# Minishell | 42Paris | As beautiful as a shell 🐚
Minishell is a project that involves creating your very own shell, taking inspiration from the Bash reference, from ground up with no pre-existing library, including key functionalities such as command execution, input/output redirection, and process management.

## Introduction
A shell, in the context of operating systems, is like a command interpreter that communicates with the operating system kernel. It allows users to execute commands, create or delete files and directories, read and write file contents, and perform various other tasks from a command line interface.

### Limitations
- No built-in library is allowed in this project. all functions must be written from scratch.
- Only one global variable is allowed. 

### Our Implementation of Minishell
The table displays the commands and the extensions were in scope of this project: 
| Builtin | Description                                            | Options | Parameters | Helpful Functions |
| ------- | ------------------------------------------------------ | ------- | ---------- | ----------------- |
| echo    | Prints arguments separated with a space followed by a new line | -n      | ✔️         | write             |
| cd      | Changes current working directory, updating PWD and OLDPWD | ❌      | ✔️         | chdir             |
| pwd     | Prints current working directory                      | ❌      | ❌         | getcwd            |
| env     | Prints environment                                     | ❌      | ❌         | write             |
| export  | Adds/replaces variable in environment                  | ❌      | ✔️         | ❌                |
| unset   | Removes variable from environment                      | ❌      | ✔️         | ❌                |

Our Minishell implementation involves several key components:
### Lexer and Expander
- The lexer and expander handle tasks like expanding environment variables (e.g., $USER), expanding '~' to the user's home directory, and tokenizing the input string.
- We split the input string into tokens, taking spaces and quotes into account, and then apply expansion functions to each substring.
- For example, the string: echo "hello there" how are 'you 'doing? $USER |wc -l >outfile is split into: {echo, "hello there", how, are, 'you 'doing?, $USER, |wc, -l, >outfile, NULL}. After expansion, it becomes: {echo, "hello there", how, are, 'you 'doing?, pixel, |wc, -l, >outfile, NULL}

### Parser
- The parser stores the tokenized and expanded string in a linked list of commands.
- It creates a chain list data structure that holds information about each command, including the command name, parameters, full path to the executable, input and output file descriptors, environment variables, and process ID:
- <img width="408" alt="Screen Shot 2023-09-01 at 2 10 18 PM" src="https://github.com/shimazadeh/Minishell/assets/67879533/73182432-1555-4976-9b27-66eb6c1115bb">


### Executor
- The executor uses the data provided by the parser to execute commands.
- It handles both built-in commands and external commands by creating child processes with proper input and output redirection.
- After all commands have executed, the exit status of the most recently executed command is retrieved.

## Installation

### Prerequisites
Ensure you have the following packages installed:
- gcc
- make
- python-norminette
- readline (valgrind on Linux)

**Note for MacOS:** To install a recent version of readline, use Homebrew: `brew install readline`

### Cloning the Repository
- git clone https://github.com/shimazadeh/minishell.git minishell
- cd minishell
- make

## Basic Usage
The project compiles an executable called `minishell` saved inside a bin/ folder), and it is compiled using the .c files inside the src/ folder

## Demos
Here are some demos of the project's progress:
- [Version 1.0](link-to-demo-v1.0)
- [Version 2.0](link-to-demo-v2.0)
- [Version 3.0](link-to-demo-v3.0)

## References
- [Minishell - 42 Docs](link-to-42-docs)
- [Bash Reference Manual](link-to-bash-manual)
- [Writing Your Own Shell](link-to-shell-writing-guide)

