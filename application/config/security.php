<?php
defined('BASEPATH') OR exit('No direct script access allowed');


$config['security'][] = ["url" => "/^admin/", "rol" => [1]];
$config['security'][] = ["url" => "/^emulador/", "rol" => [3,1]];
$config['security'][] = ["url" => "/^juegos/", "rol" => [3,1,0]];
