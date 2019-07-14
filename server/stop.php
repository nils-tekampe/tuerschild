<?php
$fp = fopen('state.txt', 'w');
fwrite($fp, '1');
fclose($fp);
http_response_code(200);
?>
