<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Main_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
        $this->load->library('encryption');
    }

    /**
     * This method is used in order to show index view.
     */
    public function index() {
        $data['title'] = "FreeBird · Juega y a volar";
        echo $this->templates->render('main::index', ['sess' => $this]);
    }

    /**
     * This method is used in order to show chat view.
     */
    public function chat() {
        echo $this->templates->render('chat::chat', ['sess' => $this]);
    }

    /**
     * This method is used in order to show shop view.
     */
    public function shop() {
        echo $this->templates->render('main::shop', ['sess' => $this]);
    }

    /**
     * This method is used in order to show news view.
     */
    public function news() {
        echo $this->templates->render('main::blog', ['sess' => $this]);
    }
}
