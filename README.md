# 🐚 Minishell

`Minishell` is a simplified **Unix shell** implemented in C.  
It supports executing commands, handling pipes and redirections, and includes several built-in commands.  

This project is part of the **42 School** curriculum and focuses on process management, parsing, signals, and environment handling.

---

## 🚀 Features

- Execute simple commands with arguments  
- Support for built-in commands:
  - `cd` → Change directory  
  - `echo` → Display messages  
  - `pwd` → Print working directory  
  - `export` → Set environment variables  
  - `unset` → Remove environment variables  
  - `env` → Display environment variables  
  - `exit` → Exit the shell
- Pipe handling (`|`)  
- Input/output redirections (`>`, `>>`, `<`)  
- Heredoc support (`<<`)  
- Signal handling (`CTRL+C`, `CTRL+D`)  

---

## 📂 Project Structure

```
minishell/
│
├── src/
│   ├── main.c
│   ├── parser.c
│   ├── executor.c
│   ├── builtins.c
│   ├── redirections.c
│   └── utils.c
│
├── includes/
│   └── minishell.h
│
├── Makefile
└── README.md
```

---

## ⚙️ Installation & Compilation

1. Clone the repository:

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
```

2. Compile the project using the Makefile:

```bash
make
```

This will create an executable called `minishell`.

---

## 🧩 Usage

Run the shell:

```bash
./minishell
```

Then you can type commands just like in a normal shell:

```bash
$ echo Hello World
Hello World
$ ls -l | grep ".c"
$ cat file.txt > output.txt
$ export MY_VAR=42
$ echo $MY_VAR
42
$ exit
```

---

## 🧠 How It Works

- **Parsing:** Splits the input into tokens, handling quotes and special characters  
- **Execution:** Executes commands using `fork()`, `execve()`, and manages pipes and redirections  
- **Built-ins:** Commands implemented internally without creating new processes  
- **Heredoc:** Reads input until a delimiter is reached and redirects it as stdin  
- **Signals:** Handles `CTRL+C` and `CTRL+D` to prevent shell crashes and exit gracefully  

---

## 🛠️ Requirements

- Unix-based OS (Linux/macOS)  
- C compiler (gcc)  
- `readline` library for user input (optional, can use `read()` instead)  

---

## 🎯 What I Learned

- Shell fundamentals and command execution  
- Parsing input with quotes, pipes, and redirections  
- Process management with `fork()`, `execve()`, and `waitpid()`  
- Environment variable management  
- Signal handling and graceful exits  

---

## 🔥 Authors

👤 Rafayle Ghazaryan
👤 Alexander Schneider
📚 42 School Student
 
