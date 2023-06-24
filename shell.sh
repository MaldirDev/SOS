#!/bin/sh

# 05-huge-access.log
# 01-tiny-access.log 

# Отримання кількості звернень для кожної дати і сортування
grep -o '\[[^]]*\]' 05-huge-access.log | sed 's/\[//' | sed 's/\]//' | cut -d':' -f1 | sort | uniq -c | sort -rn | head -n 10 > tmp.txt

# Підрахунок загальної кількості звернень
total=$(awk '{ sum += $1 } END { print sum }' tmp.txt)

# Виведення результатів
listCount=1
count=1
while read -r line; do
    date=$(echo "$line" | awk '{ print $2 }')
    count=$(echo "$line" | awk '{ print $1 }')
    percentage=$(awk "BEGIN { printf \"%.2f\", ($count/$total)*100 }")
    echo "$listCount. $date - $count - $percentage%"
    listCount=$((listCount+1))
done < tmp.txt

# Видалення тимчасового файлу
rm tmp.txt