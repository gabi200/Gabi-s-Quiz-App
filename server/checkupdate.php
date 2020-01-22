<?php
$latest_version = "1.0.1.9"; // last digit is build

if ($_GET["version"] == $latest_version) {
    echo 0;
}
else {
    echo 1;
}
?>