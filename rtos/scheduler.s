.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.global	rtos_scheduler_launch
.type	rtos_scheduler_launch, %function
rtos_scheduler_launch:
	push	{r7}
	add	r7, sp, #0
	ldr r0, =rtos_thread_current
	ldr r2, [r0]
	ldr sp, [r2]
	pop {r4-r11}
	pop {r12}
	pop {r0-r3}
	add sp, sp, #4
	pop {lr}
	add sp, sp, #4
	cpsie i
	bx lr

.global	timebase_interrupt
.type	timebase_interrupt, %function
timebase_interrupt:
	cpsid i
	push {r4-r11}
	ldr r0, =rtos_thread_current
	ldr r1, [r0]
	str sp, [r1]
	ldr r1, [r1, #4]
	str r1, [r0]
	ldr sp, [r1]
	pop {r4-r11}
	cpsie i
	bx lr
