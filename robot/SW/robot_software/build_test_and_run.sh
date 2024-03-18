pushd build 
cmake ..
make 


directory="."

for exec_file in $(find "$directory" -type f -executable -name 'ut*'); do
    # Check if the file exists and is executable
    if [ -x "$exec_file" ]; then
        # Execute the file
        echo "Running $exec_file"
        "$exec_file"
    else
        echo "Skipping $exec_file as it is not executable."
    fi
done

popd