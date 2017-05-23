<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Main_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
        $this->load->library('encryption');
    }

    public function index() {
        $data['title'] = "FreeBird · Juega y a volar";
        echo $this->templates->render('main::index', ['session' => $this]);
    }

    public function chat() {
        echo $this->templates->render('chat::chat');
    }
    public function shop() {
        echo $this->templates->render('main::shop');
    }
    public function social() {
        echo $this->templates->render('main::social');
    }
    public function news() {
        echo $this->templates->render('main::news');
    }
    public function blog() {
        echo $this->templates->render('main::blog');
    }
    public function forum() {
        echo $this->templates->render('main::forum');
    }
}
