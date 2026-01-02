(progn
(defun cdar (x) "(cdar list) Equivalent to (cdr (car list))." (cdr (car x)))

(defun cddr (x) "(cddr list) Equivalent to (cdr (cdr list))." (cdr (cdr x)))
(defun caaar (x) "(caaar list) Equivalent to (car (car (car list)))." (car (car (car x))))
(defun caadr (x) "(caadr list) Equivalent to (car (car (cdar list)))." (car (car (cdr x))))
(defun cadar (x) "(cadar list) Equivalent to (car (cdr (car list)))." (car (cdr (car x))))
(defun caddr (x) "(caddr list) Equivalent to (car (cdr (cdr list)))." (car (cdr (cdr x))))
(defun cdaar (x) "(cdaar list) Equivalent to (cdar (car (car list)))." (cdr (car (car x))))
(defun cdadr (x) "(cdadr list) Equivalent to (cdr (car (cdr list)))." (cdr (car (cdr x))))
(defun cddar (x) "(cddar list) Equivalent to (cdr (cdr (car list)))." (cdr (cdr (car x))))
(defun cdddr (x) "(cdddr list) Equivalent to (cdr (cdr (cdr list)))." (cdr (cdr (cdr x))))
)
