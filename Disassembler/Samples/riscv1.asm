main:
	addi	sp,sp,-80
	sw	s0,76(sp)
	addi	s0,sp,80
	sw	zero,-20(s0)
	sw	zero,-24(s0)
	j	.L2
.L3:
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-24(s0)
	sw	a4,-52(a5)
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L2:
	lw	a4,-24(s0)
	li	a5,9
	ble	a4,a5,.L3
	sw	zero,-28(s0)
	j	.L4
.L5:
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-52(a5)
	lw	a4,-20(s0)
	add	a5,a4,a5
	sw	a5,-20(s0)
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
.L4:
	lw	a4,-28(s0)
	li	a5,9
	ble	a4,a5,.L5
	li	a5,0
	mv	a0,a5
	lw	s0,76(sp)
	addi	sp,sp,80
	jr	ra
