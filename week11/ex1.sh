# Change current directory
cd /week11

# Create a 50MB file filled with zeros
dd if=/dev/zero of=lofs.img bs=1M count=50

# Setup a loop device for the file
loop_device=$(sudo losetup --find --show lofs.img)

# Print the loop device assigned
echo "Loop device set up: $loop_device"

# Create a ext4 file system on the loop device
sudo mkfs.ext4 $loop_device

# Create a new empty directory ./lofsdisk
mkdir ./lofsdisk

# Mount the created filesystem on the mount point ./lofsdisk
sudo mount $loop_device ./lofsdisk

# Print a message indicating the mount is successful
echo "Filesystem mounted at ./lofsdisk. You can now use this filesystem."

# Add two files to the LOFS with specific content
echo "Yaroslava" | sudo tee ./lofsdisk/file1
echo "Bryukhanova" | sudo tee ./lofsdisk/file2

# Print a message indicating the files are added
echo "Files 'file1' and 'file2' added to the LOFS with specific content."

get_libs() {
    # Check if an argument is provided
    if [ -z "$1" ]; then
        echo "Usage: get_libs <binary_path>"
        return 1
    fi

    binary_path="$1"

    # Use ldd to get shared library information
    libs=$(ldd "$binary_path" | grep '/[^ ]*' -o)
    # Print the paths of shared libraries
    echo "$libs"
}


# Use the function get_libs() to get shared libraries for commands
libs_bash=$(get_libs "/bin/bash")
libs_cat=$(get_libs "/bin/cat")
libs_echo=$(get_libs "/bin/echo")
libs_ls=$(get_libs "/bin/ls")

# Copy the shared libraries to the LOFS
sudo cp --parents $libs_bash ./lofsdisk/
sudo cp --parents $libs_cat ./lofsdisk/
sudo cp --parents $libs_echo ./lofsdisk/
sudo cp --parents $libs_ls ./lofsdisk/

# Add the commands along with their shared libraries to the LOFS
sudo cp --parents /bin/bash ./lofsdisk/
sudo cp --parents /bin/cat ./lofsdisk/
sudo cp --parents /bin/echo ./lofsdisk/
sudo cp --parents /bin/ls ./lofsdisk/

# Print a message indicating the commands are added
echo "Commands 'bash', 'cat', 'echo', and 'ls' added to the LOFS with their shared libraries."

# Print shared libraries for each command
echo "Shared libraries for /bin/bash:"
echo "$libs_bash"

echo "Shared libraries for /bin/cat:"
echo "$libs_cat"

echo "Shared libraries for /bin/echo:"
echo "$libs_echo"

echo "Shared libraries for /bin/ls:"
echo "$libs_ls"

# Change the root directory of the process to the mount point of LOFS
sudo cp ./ex1 ./lofsdisk/

sudo chroot ./lofsdisk ./ex1 > ex1.txt

# Print a message indicating the program is run
echo "Program ex1 run in the LOFS. Output saved to ex1.txt."

# Run the ex1 program and append the output to ex1.txt
./ex1 >> ex1.txt

# Print a message indicating the program is run and output is appended
echo "Program ex1 run in the LOFS. Output appended to ex1.txt."
