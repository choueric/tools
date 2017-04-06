#!/usr/bin/awk -f

# from <project path="abi/cpp" name="android/platform/abi/cpp-4.1" groups="pdk" />
# to   <project path="abi/cpp" name="android_platform_abi_cpp-4-1" groups="pdk" />

{
	if ($1 ~ "<project") {
		for (i = 1; i <= NF; i++) {
			gsub(/\//,"_", $3)
			gsub(/\./,"-", $3)
			printf " %s", $i
		}
		printf "\n"
	} else {
		print $0
	}
}
