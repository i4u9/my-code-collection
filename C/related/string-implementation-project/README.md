This program takes two input files: "src.txt" and "model.txt", with these structures -- 
|
|-> src:
|   - (quantity of elements)
|   - element1
|   - element2
|   - element3
|       .
|       .
|       .
|-> model:
    - (placeholder)
    - (some atribute1): placeholder, placeholder, ...
    - (atribute2): placeholder, placeholder, ...
    - (atribute3): placeholder, placeholder, ...

What the program does is it will insert all the elements in src to model,
at the postion of the placeholders and in random order, returning the result to result.txt. Example --
|
|-> src:
|   - 3
|   - Josh
|   - Joe
|   - Jeremiah
|-> model:
    - 1
    - Server: 1
    - Recepcionist: 1
    - Inspector: 1
    |
    |
    result:
    - Server: Josh
    - Recepcionist: Jeremiah
    - Inspector: Joe
