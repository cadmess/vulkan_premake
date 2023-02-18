find . -name "*.sln" | xargs rm -f
find . -name "*.vcxproj" | xargs rm -f
find . -name "*.vcxproj.filters" | xargs rm -f
find . -name "*.vcxproj.user" | xargs rm -f
rm -r -f ".vs"
echo "cleaning finished."

function pause(){
   read -p "$*"
}

read -t 5 -p "Press any key in 5 seconds to keep window open.." key
if [[ $? -gt 128 ]] ; then
    echo -e "\nWindow exiting.."
else
    echo ""
    pause pause 'Key pressed. Keeping window open. Press [Enter] to close..'
fi
exit