inputcreate.py makes a random file called input.txt that mts.cpp reads

each line is [EWew]:[1-99],[1-99]
where EWew signals the direction and priority of the thread,
the first int signifies the load time of the thread (in ms)
the second int signifies the runtime of the thread (in ms)

invoking MAKE on the directory will create a file called mts

run with ./mts input.txt

mts is not programmed to handle incorrectly formatted data or an absent filename
