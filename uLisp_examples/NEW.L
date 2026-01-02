

(progn

 (pinmode 16 1)
 (pinmode 17 1)
 (pinmode 0 5)
 (digitalwrite 16 1)
 (digitalwrite 17 1)
 (analogread 0)

(defcode initadc ()
  initadc
  ; Load ADC address
  ($lui 'a4 #x40012)
  ($lui 'a7 #x00400)
  ($srli 'a7 'a7 12)
  ($add 'a4 'a4 'a7)

  ($li 'a7 0)   ; conversion time
  ($ori 'a7 'a7 #x03)   ; conversion time
  ($sw 'a7 #x10 '(a4))  ; Set ADC channel

  ($li 'a7 0)   ; set Rank
  ($sw 'a7 #x34 '(a4))  ; zero for channel 0

  ($ret)
)


(defcode read-adc-array (pause address n)
  read-adc-array
  ($addi 'sp 'sp -8)
  ($sw 'ra 0 '(sp))
  ($sw 'a0 4 '(sp))

  ; Load ADC address
  ($lui 'a4 #x40012)
  ($lui 'a6 #x00400)
  ($srli 'a6 'a6 12)
  ($add 'a4 'a4 'a6)

  ; Load GPIO_A address
  ($lui 'a5 #x40010)
  ($lui 'a6 #x00c00)
  ($srli 'a6 'a6 12)
  ($add 'a5 'a5 'a6)

  ; Double pause before start pulse bit2
  ($lw 'a0 4 '(sp))
  ($add 'a0 'a0 'a0)
  ($jal pause0)

  ; Clear GPIO_A bit2
  ($li 'a3 #x00000002)
  ($sw 'a3 #x00000014 '(a5))

  ; 10 pauses start pulse bit2
  ($lw 'a0 4 '(sp))
  ($li 'a6  10 )
  ($mul 'a0 'a0 'a6)
  ($jal pause0)

  ; Rise GPIO_A bit2
  ($sw 'a3 #x00000010 '(a5))

  ; Double pause after start pulse bit2
  ($lw 'a0 4 '(sp))
  ($li 'a6  10 )
  ($mul 'a0 'a0 'a6)
  ($jal pause0)

  ; Set bit_1 mask GPIO_A
  ($li 'a3 #x00000001)


  read-array-loop  ;  Main loop
  ($beqz 'a2 ret2) ; a2==0 -> return

  ; Clear GPIO_A bit_1
  ($sw 'a3 #x00000014 '(a5))

  ; Start ADC conversion
  ($lw 'a6  #x08 '(a4))
  ($lui 'a0 #x00500)
  ($or 'a6 'a6 'a0)
  ($sw 'a6 #x08 '(a4))

  ; first pause
  ($lw 'a0 4 '(sp))
  ($jal pause0)


  ; Rise GPIO_A bit_1
  ($sw 'a3 #x00000010 '(a5))

  ; second pause
  ($lw 'a0 4 '(sp))
  ($jal pause0)

  ; Read ADC value
  ($lw 'a6 #x04C '(a4))
  ($sw 'a6 0 '(a1))

  ($addi 'a1 'a1  4)  ; Increment store address
  ($addi 'a2 'a2 -1)
  ($j read-array-loop)

  ret2
  ($lw 'ra 0 '(sp))
  ($addi 'sp 'sp 8)
  ($ret)

  pause0  ; Pause subroutine
  ($beqz 'a0 ret1)
  ($addi 'a0 'a0 -1)
  ($j pause0)
  ret1
  ($ret)
)

(defcode readx (address n)
  readx
  ($li 'a2 4)
  ($mul 'a1 'a1 'a2)
  ($add 'a1 'a1 'a0)
  ($lw 'a0 0 '(a1))
  ($ret)
)

 (defvar SENSOR_ARRAY_SIZE 2048)
 (defvar sensor-data2 0)
 (defvar sensor-data (make-array SENSOR_ARRAY_SIZE :initial-element 0))
  

 (dotimes
  (i SENSOR_ARRAY_SIZE)
  (setf
   (aref sensor-data i)
   (*
    (+ 1.0
     (sin
      (/
       (* 3.1415 i) 500.0))) 2047)))


 
 (defun read-adc nil
  (digitalwrite 16 0)
  (digitalwrite 16 1)
  (analogread 0))
  
 (defun read-sensor nil
  (digitalwrite 17 0)
  (digitalwrite 17 1)
  (dotimes
   (i SENSOR_ARRAY_SIZE)
   (setf
    (aref sensor-data i)
    (read-adc))))
   
 (defun read-sensor2 (p)
   (initadc)
   (read-adc-array p sensor-data2 2048)
 )
 
 (defun Draw-Data nil
  (fill-rect 0 0
   (getscrmaxx)
   (getscrmaxy) 0)
  (let*
   (
    (address  sensor-data2)
    (scr_xsize
     (1-
      (getscrmaxx)))
    (scr_ysize
     (getscrmaxy))
    (data_index 0)
    (data_ind_coef
     (*
      (/ 1.0 scr_ysize) SENSOR_ARRAY_SIZE))
    (xpix 0)
    (data_color 31)
    (xpix_prev
     (truncate (/ (readx sensor-data2  0) 15))))
   (dotimes
    (i
     (1- scr_ysize))
    (setq data_index
     (round
      (* data_ind_coef
       (1+ i))))
    (setq xpix
      (truncate (/ (readx sensor-data2  data_index) 15))
    )
    (draw-line xpix_prev i xpix i data_color)
    (setq xpix_prev xpix))))

(defun view-adc () 
 (if (eq sensor-data2 0)(setq sensor-data2 (allocmem  (* 4 (+ SENSOR_ARRAY_SIZE 1)))))
 (loop
  (read-sensor2 50)
  (Draw-Data)
  (if
   (kbhit)
   (return))
  (if
   (touch-press)
   (return)))
 )
)
