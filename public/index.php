<?php

require __DIR__ . '/vendor/autoload.php';
use FreeBird\controllers\ArticlesController;

$controller = new ArticlesController();
$controller->index();