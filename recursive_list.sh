#!/bin/bash

# 함수 정의: 재귀적으로 디렉토리를 방문
list_directory() {
    local dir=$1

    # 현재 디렉토리 출력
    echo "Directory: $dir"
    echo "-----------------"
    ls -1 "$dir"  # 현재 디렉토리의 파일과 디렉토리 목록 출력
    echo ""

    # 디렉토리 내부 항목 순회
    for item in "$dir"/*; do
        if [ -d "$item" ]; then
            # 서브디렉토리인 경우 재귀 호출
            list_directory "$item"
        fi
    done
}

# 실행 시작
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Error: $1 is not a valid directory"
    exit 1
fi

# 지정된 디렉토리에서 함수 호출
list_directory "$1"


