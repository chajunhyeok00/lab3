#!/bin/bash

# 연습 문장 배열 정의
sentences=(
    "The quick brown fox jumps over the lazy dog."
    "Practice makes perfect."
    "Linux is a powerful operating system."
)

# 변수 초기화
total_typing_time=0
total_words=0
total_errors=0

echo "타자 연습 프로그램에 오신 것을 환영합니다!"
echo "각 문장을 입력하세요. 오타 수와 WPM이 계산됩니다."
echo ""

for sentence in "${sentences[@]}"; do
    echo "문장: $sentence"
    echo -n "입력: "

    # 타이핑 시작 시간 기록
    start_time=$(date +%s)

    # 사용자 입력 받기
    read -r user_input

    # 타이핑 종료 시간 기록
    end_time=$(date +%s)

    # 시간 계산
    elapsed_time=$((end_time - start_time))
    total_typing_time=$((total_typing_time + elapsed_time))

    # 단어 수 계산
    word_count=$(echo "$sentence" | wc -w)
    total_words=$((total_words + word_count))

    # 오타 계산
    errors=$(diff <(echo "$sentence") <(echo "$user_input") | grep "<" | wc -l)
    total_errors=$((total_errors + errors))

    echo "걸린 시간: ${elapsed_time}초"
    echo "오타: $errors개"
    echo ""
done

# 평균 WPM 계산
if [ $total_typing_time -gt 0 ]; then
    average_wpm=$((total_words * 60 / total_typing_time))
else
    average_wpm=0
fi

# 결과 출력
echo "======== 결과 ========"
echo "총 오타 수: $total_errors개"
echo "평균 WPM: $average_wpm 단어/분"
echo "======================="

