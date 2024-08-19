#bin\bash

i=1
suc=0
fail=0
#TEST 1
grep asd test.txt > 1
./s21_grep asd test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 2
i=$((i+1))
grep -n asd test.txt > 1
./s21_grep -n asd test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 3
i=$((i+1))
grep -e asd test.txt > 1
./s21_grep -e asd test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 4
i=$((i+1))
grep -e asd -e bv test.txt > 1
./s21_grep -e asd -e bv test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 5
i=$((i+1))
grep -i ASD  test.txt > 1
./s21_grep -i ASD test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 6
i=$((i+1))
grep -v asd  test.txt > 1
./s21_grep -v asd test.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 7
i=$((i+1))
grep -l asd  test.txt s21_grep.c > 1
./s21_grep -l asd test.txt s21_grep.c > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 8
i=$((i+1))
grep -h asd  test.txt test2.txt > 1
./s21_grep -h asd test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 9
i=$((i+1))
grep -s asd  test.txt est.txt > 1
./s21_grep -s asd test.txt est.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 10
i=$((i+1))
grep asd test.txt est.txt > 1
./s21_grep asd test.txt est.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 11
i=$((i+1))
grep -f regular.txt  test.txt test2.txt > 1
./s21_grep -f regular.txt test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 12
i=$((i+1))
grep -o asd  test.txt test2.txt > 1
./s21_grep -o asd test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 13
i=$((i+1))
grep -o -e asd -e bv test.txt test2.txt > 1
./s21_grep -o -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 14
i=$((i+1))
grep -l -e asd -e bv test.txt test2.txt > 1
./s21_grep -l -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 15
i=$((i+1))
grep -v -e asd -e bv test.txt test2.txt > 1
./s21_grep -v -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 16
i=$((i+1))
grep -vc -e asd -e bv test.txt test2.txt > 1
./s21_grep -vc -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 17
i=$((i+1))
grep -vlc -e asd -e bv test.txt test2.txt > 1
./s21_grep -vlc -e asd -e bv test.txt test2.txt > 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi


#TEST 18
i=$((i+1))
grep -vo -e asd -e bv test.txt test2.txt > 1
./s21_grep -vo -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 19
i=$((i+1))
grep -f regular.txt -ivch -e asd -e bv test.txt test2.txt > 1
./s21_grep -f regular.txt -ivch -e asd -e bv test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 20
i=$((i+1))
grep -f regular.txt -ivch -e asd -e рус test.txt test2.txt > 1
./s21_grep -f regular.txt -ivch -e asd -e рус test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 21
i=$((i+1))
grep -f regular.txt -in -e asd -e рус test.txt test2.txt > 1
./s21_grep -f regular.txt -in -e asd -e рус test.txt test2.txt > 2
diff 1 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 22
i=$((i+1))
grep > 1
./s21_grep > 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 23
i=$((i+1))
grep -f regular.txt -i test.txt > 1
./s21_grep -f regular.txt -i test.txt > 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi

#TEST 24
i=$((i+1))
grep -f regula.txt -i test.txt > 1
./s21_grep -f regula.txt -i test.txt > 2

if [ $? -eq  0 ]
then
    echo "Test $i: SUCCESS"
    suc=$((suc+1))
else
    echo "Test $i: FAIL"
    fail=$((fail+1))
fi
