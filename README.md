<h1 align=center>
unique_words
</h1>

A program that counts the number of distinct unique words in a file whose name is passed as an argument to a program. 
Program uses a simple thread pool in order to utilize all available CPU resources.

## To build and run

```powershell
cmake -S . -B build
cd build
cmake --build . --config Release
./unique_words file
```
