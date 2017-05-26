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
        echo $this->templates->render('main::index', ['sess' => $this]);
    }

    public function chat() {
        echo $this->templates->render('chat::chat', ['sess' => $this]);
    }
    public function shop() {
        echo $this->templates->render('main::shop', ['sess' => $this]);
    }
    public function social() {
        echo $this->templates->render('main::social', ['sess' => $this]);
    }
    public function news() {
        echo $this->templates->render('main::news', ['sess' => $this]);
    }
    public function blog() {
        echo $this->templates->render('main::blog', ['sess' => $this]);
    }
    public function forum() {
        echo $this->templates->render('main::forum', ['sess' => $this]);
    }
}
