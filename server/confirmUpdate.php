<?php
$fp = fopen('last_update.txt', 'w');
$date = (new DateTime('NOW'))->format("d:m:y H:i:s");
fwrite($fp, $date);
fclose($fp);
http_response_code(200);
?>
