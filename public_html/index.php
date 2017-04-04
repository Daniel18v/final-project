<?php
require_once __DIR__ . "/src/controllers/articlesController.php";

$controller = new articlesController();
$controller->index();