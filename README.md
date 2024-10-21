<h1 align=center>
unique_words
</h1>

A program that counts the number of distinct unique words in a file whose name is passed as an argument to a program. Program uses simple thread pool in order to utilize available CPU resources.

## To build

```powershell
cmake -S . -B build
cmake --build build
```

## To run

```powershell
./unique_words file_name
```
