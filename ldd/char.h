#ifndef __CHAR_H_INCLUDE__
#define	__CHAR_H_INCLUDE__

#define SCULLC_MAJOR 0   /* dynamic major by default */

#define SCULLC_DEVS 4    /* scullc0 through scullc3 */
#define SCULLC_QUANTUM  4000 /* use a quantum size like scull */
#define SCULLC_QSET     500

struct scullc_dev {
	void **data;
	struct scullc_dev *next;  /* next listitem */
	int vmas;                 /* active mappings */
	int quantum;              /* the current allocation size */
	int qset;                 /* the current array size */
	size_t size;              /* 32-bit will suffice */
	struct semaphore sem;     /* Mutual exclusion */
	struct cdev cdev;
};

#endif