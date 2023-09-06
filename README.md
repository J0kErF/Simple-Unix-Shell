# Simple Unix Shell
A simple shell written in C.

## Features

* Supports basic commands such as `cd`, `ls`, `pwd`, `exit`.
* Can run commands from the history list.
* Supports background execution of commands.

## How to use

1. Clone the repository.
2. Compile the code with the following command:

gcc my_shell.c -o my_shell


3. Run the shell with the following command:

`./my_shell`

Enter the commands you want to run.
### History
The shell maintains a history of all the commands that have been run. You can access the history list by typing history.

### Background execution
You can run a command in the background by appending an & to the end of the command. For example, the following command will run the ls command in the background:
`ls &`

To view the list of background processes, you can type `jobs`.

## Exiting the shell

To exit the shell, type `exit`.

## License

The my_shell is released under the MIT License.
