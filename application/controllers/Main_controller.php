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
        $this->load->model('Blog_model', 'mod');
    }

    /**
     * This method is used in order to show index view.
     */
    public function index() {
        $data['title'] = "FreeBird · Juega y a volar";
        $blogs = $this->mod->get_blogs_index();
        empty(!$blogs) ? : show_404();
        echo $this->templates->render('main::index', ['sess' => $this, 'blogs' => $blogs]);
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
        $blogs = $this->mod->get_blogs();
        empty(!$blogs) ? : show_404();
        echo $this->templates->render('main::news', ['sess' => $this, 'blogs' => $blogs]);
    }
}
