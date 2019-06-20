<?php
$fp = fopen('state.txt', 'w');
fwrite($fp, 'stop');
fclose($fp);
http_response_code(200);
?>
