transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/_gate.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/mx8_32bits.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/mx2_32bits.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/fa_v2.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/clb4.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/cla4_ov.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/cla4.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/cla32_ov.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/cal_flags32.v}
vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/alu32.v}

vlog -vlog01compat -work work +incdir+C:/Users/J/Desktop/alu32 {C:/Users/J/Desktop/alu32/tb_alu32.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneii_ver -L rtl_work -L work -voptargs="+acc"  tb_alu32

add wave *
view structure
view signals
run -all
