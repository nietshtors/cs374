REGEX="<a\s+(?:[^>]*?\s+)?href=([\"'])\K(.*?)\1"
curl $1 -s | grep -oP $REGEX | grep -oP ".*[^\"']"