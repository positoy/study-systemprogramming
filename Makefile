#CC=gcc
#TARGET=exefile
#OBJECT=a.o b.o main.o

#$(TARGET):	$(OBJECT)
	#$(CC) -o(TARGET) $(OBJECT)

all: file_pointer.out file_descriptor.out link.out stat.out directory.out mmap.out flock.out fcntl.out inotify.out select.out epoll_basic.out epoll.out fork.out affinity.out pthread.out mutex.out signal.out adv_signal_send.out adv_signal_recv.out time.out timer.out

file_pointer.out: 01_file_pointer.c
	gcc -o file_pointer.out -g -Wall 01_file_pointer.c

file_descriptor.out: 02_file_descriptor.c
	gcc -o file_descriptor.out -g -Wall 02_file_descriptor.c

link.out: 03_link.c
	gcc -o link.out -g -Wall 03_link.c

stat.out: 04_stat.c
	gcc -o stat.out -g -Wall 04_stat.c

directory.out: 05_directory.c
	gcc -o directory.out -g -Wall 05_directory.c

mmap.out: 06_mmap.c
	gcc -o mmap.out -g -Wall 06_mmap.c

flock.out: 07_flock.c
	gcc -o flock.out -g -Wall 07_flock.c

fcntl.out: 08_fcntl.c
	gcc -o fcntl.out -g -Wall 08_fcntl.c

inotify.out: 09_inotify.c
	gcc -o inotify.out -g -Wall 09_inotify.c

select.out: 10_select.c
	gcc -o select.out -g -Wall 10_select.c

epoll_basic.out: 11_epoll_basic.c
	gcc -o epoll_basic.out -g -Wall 11_epoll_basic.c

epoll.out: 11_epoll.c
	gcc -o epoll.out -g -Wall 11_epoll.c
	
fork.out:	12_fork.c
	gcc -o fork.out -g -Wall 12_fork.c

affinity.out:	13_affinity.c
	gcc -o affinity.out -g -Wall 13_affinity.c

pthread.out: 14_pthread.c
	gcc -o pthread.out -g -Wall 14_pthread.c -lpthread

mutex.out: 15_mutex.c
	gcc -o mutex.out -g -Wall 15_mutex.c -lpthread

signal.out: 16_signal.c
	gcc -o signal.out -g -Wall 16_signal.c

adv_signal_send.out: 17_signal_advanced.c
	gcc -o adv_signal_send.out -g -Wall 17_signal_advanced.c -DSEND

adv_signal_recv.out: 17_signal_advanced.c
	gcc -o adv_signal_recv.out -g -Wall 17_signal_advanced.c

time.out: 18_time.c
	gcc -o time.out -g -Wall 18_time.c

timer.out: 19_timer.c
	gcc -o timer.out -g -Wall 19_timer.c

clean :
	rm *.out

	
