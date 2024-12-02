#!/bin/bash

# 1. 디렉토리 생성
dir_name="example_dir"
if [ ! -d "$dir_name" ]; then
    mkdir "$dir_name"
    echo "디렉토리 '$dir_name'이(가) 생성되었습니다."
else
    echo "디렉토리 '$dir_name'이(가) 이미 존재합니다."
fi

# 2. 디렉토리 내용 확인
echo -e "\n현재 디렉토리 내용:"
ls

# 3. 파일 생성 및 내용 작성
file_path="$dir_name/example_file.txt"
echo "안녕하세요, 파일입니다!" > "$file_path"
echo "파일 '$file_path'이(가) 생성되고 내용이 작성되었습니다."

# 4. 파일 내용 출력
echo -e "\n파일 내용:"
cat "$file_path"

# 5. 파일 및 디렉토리 삭제
rm "$file_path"
echo "파일 '$file_path'이(가) 삭제되었습니다."
rmdir "$dir_name"
echo "디렉토리 '$dir_name'이(가) 삭제되었습니다."

