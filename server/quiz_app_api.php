<?php
date_default_timezone_set("Europe/Bucharest");
$line = date('Y-m-d H:i:s') . " - $_SERVER[REMOTE_ADDR]";
file_put_contents('visitors_api.log', $line . PHP_EOL, FILE_APPEND);

function upload() {
    $spaces = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    chdir("..");
    $file = fopen("quiz_app_db.html", "a") or die("Unable to open file");
    $data = "<b>" . $_GET["user"] . $spaces . $_GET["questions"] . $spaces . $_GET["score"] . $spaces . $_GET["id"] ."</b><br>\n";
    fwrite($file, $data);
    fclose($file);
}

chdir("users");

if (file_exists($_GET["user"])) {
    $file1 = fopen($_GET["user"], "r");
    if (fgets($file1) == hash("sha256", $_GET["pwd"])){
        upload();
        echo "Authentication sucessful!";
    }
    else {
        echo "Authentication failed: wrong password!";
    }
}
else {
    $file2 = fopen($_GET["user"], "w");
    fwrite($file2, hash("sha256", $_GET["pwd"]));
    upload();
}
?>
