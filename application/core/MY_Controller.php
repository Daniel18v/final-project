<?php

defined("BASEPATH") or exit("No direct script access allowed");

class MY_Controller extends CI_Controller {

    public $templates;

    public function __construct() {
        parent::__construct();
        $this->templates = new League\Plates\Engine(APPPATH . "/views");
        $this->templates->addFolder('games', APPPATH . '/views/games');
        $this->templates->addFolder('templates', APPPATH . '/views/templates');
        $this->templates->addFolder('structure', APPPATH . '/views/structure');
        $this->templates->addFolder('chat', APPPATH . '/views/chat');
    }

}