<?php

// Afficher les informations de l'environnement CGI
foreach ($_SERVER as $key => $value) {
    echo $key . ": " . $value . "<br>";
}
?>
