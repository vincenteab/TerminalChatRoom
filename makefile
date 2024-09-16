tester:
	gcc -Wall -Werror -o s-talk s-talk.c input.c sender.c receiver.c output.c socketHandler.c list.c -lpthread

clean:
	rm -f s-talk