# COP4520assignment3

To run part 1:
g++ -fpermissive mino.cpp

./a.out

To run part 2:
g++ -fpermissive temp.cpp

./a.out


# Part 1
By default this part will not print any of it's progress, and will only print when the minotaur asks to check the list (which he arbitrarily does 100 times, that can be changed on line 182) and when the servants are finished. To track progress, uncomment lines 73, 100, 114, 127. However this will make it run very slowly while it prints. Without printing it runs in less than a second. I recommend reducing the present amount (#define NPRESENTS) first, so you can see how it runs without waiting for over 500,000 prints.

The solution was very simple since it is exactly what the minotaur asked; each servant alternates between putting a present in the list, and taking it off to write a note. This way the list has almost no functional purpose since there is usually nothing on it since the servent takes it back off immediately since they were asked to alternate actions, but that is what the minotaur said to do so they do it ("every servant was asked to alternate adding gifts to the ordered chain and writing “Thank you”
cards." -from the assignment instructions). The minotaur asking if a present is on the list is also pretty useless, since the answer is almost always "no". Ocassionally the question may come in during the brief window where the list is unlocked but the present has not yet been removed, but it is a rare occurrance.

# Part 2
An end time was not specified for this part so by default it runs for 10 hours, but that can be changed with #define HOURS_DURATION on line 7, to run it any number of hours. 

My solution works by storing the temperature readings in an array, which has 8 * 60 spots (8 sensors, a reading per minute, and we calculate every hour so we only need to store that many readings). Each sensor has it's own slots, in increments of 8, eliminating the need for locks since they will never have race conditions this way which improves speed by not having to ever wait for another sensor since another sensor will never try to save its reading to another's slot. After the 8 * 60 readings are taken, the 5 maximum, 5 minimum, and highest range are calculated by the CPU on the rover. After those calculations, the temperature array is overwritten for the next hour's temperatures to reduce storage costs. To simulate the default 10 hours, it takes less than a second on my computer. To do for example 2 days worth of readings (48 hours) it takes just over 1 second. A whole week's worth of readings (168 hours) takes just under 4 seconds, which is quite efficient.
