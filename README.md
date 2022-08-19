# 61C Spring 2022 Project 1: Snake

Spec: [https://cs61c.org/sp22/projects/proj1/](https://cs61c.org/sp22/projects/proj1/)

Task1: 5 hours Task2 and Task 3: 0.5 hour Task4: 6 hours Task 5: 3 hours Task 6: 2 hours Task 7: 0.5 hour Debugging: 5 hours

 A playable snake game where snakes are placed on a board. Snakes grow if they eat fruits, and they die if they move into a wall.

There are two time-consuming bugs I encountered. The first one is segmentation fault: I accessed a memory location that is outside the bound of the board. Because in C many things could cause segmentation fault, it took me a while to isolate the line of code that is seg-faulting and found out the reason why it is a segmentation fault and fix it in the end. The other one is messing up row number and column number with x and y coordinates of the board. This bug caused update_state to have weird hebaviors and snakes does not move/advance according to the spec. I like all parts of the project and I cannot really pick one that is the most fun part. One thing that I think could be improved is the order of tasks: create_default_state() is actaully a lengthy and difficult task as it requires a comprehensive understanding about the setup of the game and the state and snake struct. So I think it could be divided into smaller tasks or put it back a little. I think snek is a really fun game!
