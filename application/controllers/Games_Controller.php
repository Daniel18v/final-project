<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Games_Controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('Games_Model');
        $this->load->helper('url');
    }

    /*
     * Index is the main page, and in it charges
     * all titles for show them in the view.
     */

    public function index() {
        $data['all'] = $this->Games_Model->get_all();
        $data['title'] = "FreeBird · Juega y a volar";
        if (empty($data['all'])) {
            show_404();
        }
        echo $this->templates->render('games::index', $data);
    }

}
