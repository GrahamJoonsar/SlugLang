# SlugLang
A simple interpreted programming language. The read=son behind the name is the language's methodology, "One step at a time". Lots of things that would be one line in other languages are seperated into multiple lines for clarity. There should only be one operation, or something happening, per line, although technically there can be more.

# Documentation
A brief overview on how to use the slug programming language

## Declaring variables

Declaring variables is technically calling a function, but I will seperate it for clarity. There are three base datatypes in Slug, `int`, `float`, and `string`.

`int` is used for whole numbers, or integers.

`float` is used for numbers with fractional components.

`string` is used for words or letters.

Here is the code used to declare these three datatypes.

![declarations](https://user-images.githubusercontent.com/70826772/121429336-caa98b00-c944-11eb-864f-2321a470c650.png)

There is another, more complicated datatype to declare, the `bool`. Intead of passing in true or false as most other languages, there is a different way of doing it in slug. First, you pass in the name of the `bool`, Next, the types of values you are declaring. Third, You put in the first value/variable to be compared. Fourthly, the dev puts in the boolean operator, like `==` or `>`. Lastly you put in the second value/variable to be compared. Here is an example.

`bool myBool int 3 > 0`

Here is the code for making bools.

![boolDeclaration](https://user-images.githubusercontent.com/70826772/121434120-d5671e80-c94a-11eb-8587-78993f4f3009.png)


## Base functions

`print` is the function that outputs to the console. It can either output a string literal, an `int`, a `float`, and a `string`. It automatically adds a newline to the end of what you give it.

## Examples

Here is an example program that greets the user.

![carbon (1)](https://user-images.githubusercontent.com/70826772/121422408-3687f580-c93d-11eb-85ec-9401ecac4ffa.png)

The first line starts with a hashtag/octothorpe. This indicates a comment, similar to python.

Then, a line is printed with a newline at the end using the `println` function. This takes in a string literal or a variable.

Next, the program takes input in the form of a string using the `readStr` function. This puts the user input into the string var specified.

Then the string literal `$"Hi "` is outputted using the `print` statement, which does not output a new line.

Next, whatever the user inputted is outputted by `print`.

Finally, the string literal `$"!"` is outputted with a newline using `println`.

File:

[greeting.slug](Examples/greeting.slug)

#### Notes:

All string literals must be preceded with a `$`. This is so strings aren't confused with variable names

Variables do not have to be declared before use. This is because of how variables are stored in the interpreter.

The interpreter uses a map that uses a `std::string` key and whatever type the dev specifies as the value type. So whenever the developer sets a variable, the value in the dictionary corresponding to the key (variable name) is set to the value the dev provides. This way, all variables are global, but I might change this later.

![variables](https://user-images.githubusercontent.com/70826772/121426767-f5461480-c941-11eb-8f9a-5a8e1b4c17bc.png)

