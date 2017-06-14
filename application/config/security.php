<?php
defined('BASEPATH') OR exit('No direct script access allowed');


$config['security'][] = ["url" => "/^admin/", "rol" => [1]];
$config['security'][] = ["url" => "/^nes/", "rol" => [3,1]];
$config['security'][] = ["url" => "/^sms/", "rol" => [3,1]];
$config['security'][] = ["url" => "/^juegos/", "rol" => [3,1,0]];
$config['security'][] = ["url" => "/^perfil/", "rol" => [3,1]];
