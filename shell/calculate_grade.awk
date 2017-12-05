#!/usr/bin/awk -f

function getGrade(point) {
	if (point >= 96 && point <= 100) {
		return 4.8
	} else if (point >= 93 && point <= 95) {
		return 4.5
	} else if (point >= 90 && point <= 92) {
		return 4.0
	} else if (point >= 86 && point <= 89) {
		return 3.8
	} else if (point >= 83 && point <= 85) {
		return 3.5
	} else if (point >= 80 && point <= 82) {
		return 3.0
	} else if (point >= 76 && point <= 79) {
		return 2.8
	} else if (point >= 73 && point <= 75) {
		return 2.5
	} else if (point >= 70 && point <= 72) {
		return 2.0
	} else if (point >= 66 && point <= 69) {
		return 1.8	
	} else if (point >= 63 && point <= 65) {
		return 1.5
	} else if (point >= 60 && point <= 62) {
		return 1.0
	} else {
		return 0
	}
}

BEGIN {
	grade_sum=0
	point_sum=0
}

{
	grade = getGrade($3)
	grade_sum += grade * $2
	point_sum += $2
	print NR, $1, $2, $3, grade
}

END {
	print point_sum, grade_sum / point_sum
}
