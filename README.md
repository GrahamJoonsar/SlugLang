# SlugLang
A simple interpreted programming language. The reason behind the name is the language's methodology, "One step at a time". Lots of things that would be one line in other languages are seperated into multiple lines for clarity. There should only be one operation, or something happening, per line, although technically there can be more. Also, Slug has a lot less characters used for proccesses, like declaring a variable. For example, `int num 5` is used instead of `int num = 5`, as the equal sign is assumed.

Take a look at some [Examples](#examples)

## More info

I released a slug extension on the VSCode marketplace for syntax hilighting. So far it is updated for version 1.0. To find it, just search SLUG in the extensions searchbar.

# Documentation
A brief overview on how to use the slug programming language

## Declaring variables

Declaring variables is technically calling a function, but I will seperate it for clarity. There are three base datatypes in Slug, `int`, `float`, and `string`.

`int` is used for whole numbers, or integers.

`float` is used for numbers with fractional components.

`string` is used for words or letters.

Here is the code used to declare these three datatypes.

![carbon (1)](https://user-images.githubusercontent.com/70826772/125175813-ab4c8a80-e19c-11eb-9c78-06081c792c5a.png)

There is another, more complicated datatype to declare, the `bool`. Intead of passing in true or false as most other languages, there is a different way of doing it in slug. First, you pass in the name of the `bool`, Next, you put in the first value/variable to be compared. Then, you put in the boolean operator, like `==` or `>`. Lastly you put in the second value/variable to be compared. Here is an example.

`bool myBool 3 > 0`

## Base functions

`print` is the function that outputs to the console. It can either output a string literal, an `int`, a `float`, and a `string`. It does not add a newline after what you give it.

`println` is the same as print but it adds a newline at the end.

![printandprintln](https://user-images.githubusercontent.com/70826772/125176271-2f544180-e1a0-11eb-9b99-bf8f77f9437b.png)

`Output: Hello World! (With a newline)`

`printf` is the preferred way of outputting to the console. It takes in a varied amount of arguments, and prints them.

![printf](https://user-images.githubusercontent.com/70826772/125176154-49d9eb00-e19f-11eb-8276-27921f38de80.png)

For getting user input, there are three main functions. `readInt`, `readFloat`, and `readStr`. These are pretty self-explanatory, so I won't go in depth. These functions take input in the forms of the three main datatypes. The only parameter they take is the name of the variable to put the input in.

![input](https://user-images.githubusercontent.com/70826772/125176611-bb676880-e1a2-11eb-903d-cf25fbc060bc.png)

Next are the main conditional functions `if`, `elseif`, `else`. They are basically the same as other languages, but they are technically functions. They take one parameter, a boolean operation. To tell what is under the if statements, the slug language uses 4 spaces for indentation, like a tab in python.

![ifStatements](https://user-images.githubusercontent.com/70826772/125179656-499d1800-e1be-11eb-8131-5bbaf2660278.png)

An example of a full if statement is in Examples.

## Examples

Here is an example program that greets the user.

![carbon (1)](https://user-images.githubusercontent.com/70826772/121422408-3687f580-c93d-11eb-85ec-9401ecac4ffa.png)

The first line starts with a hashtag/octothorpe. This indicates a comment, similar to python.

Then, a line is printed with a newline at the end using the `println` function. This takes in a string literal or a variable.

Next, the program takes input in the form of a string using the `readStr` function. This puts the user input into the string var specified.

Then the string literal `$"Hi "` is outputted using the `print` statement, which does not output a new line.

Next, whatever the user inputted is outputted by `print`.

Finally, the string literal `"!"` is outputted with a newline using `println`.

File:

[greeting.slug](Examples/greeting.slug)

#### Notes:

Variables do not have to be declared before use. This is because of how variables are stored in the interpreter.

The interpreter uses a map that uses a `std::string` key and whatever type the dev specifies as the value type. So whenever the developer sets a variable, the value in the dictionary corresponding to the key (variable name) is set to the value the dev provides. This way, all variables are global, but I might change this later.

![variables](https://user-images.githubusercontent.com/70826772/121426767-f5461480-c941-11eb-8f9a-5a8e1b4c17bc.png)


Here is an example of an if statement.

![betterIf](https://user-images.githubusercontent.com/70826772/121551612-caa99980-c9dd-11eb-9743-37651d87202c.png)

First, the program declares an `int` called `age`. Although unneccesary, this shows how to properly declare a variable in slug. Then the program prompts the user for their age, which is stored in the variable `age`. Then, two bools are created for the if statements in the program. The first, `over18` is the bool that represents if the user is >= 18 years old. Secondly, the `bool` `is17` tells if the user is 17 years old. Then the program moves onto the if statements. If `over18` is true, then the user is prompted to vote for either apples or cheese. Their input is tested into bools that check if they voted for apples or cheese. If they voted apple, the program says. else if they voted for cheese, the program says. If `over18` is false and `is17` is true (elseif), It says "So close". Else, it says they are too young.

[voting.slug](Examples/voting.slug)


