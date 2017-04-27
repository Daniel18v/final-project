<?php

class Games_Controller extends CI_Controller {

    /*
     * Loads model and helper for controller.
     */

    function __construct() {
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
        $data['title'] = "Esto es un título";
        if (empty($data['all'])) {
            show_404();
        }
        $this->load->view('templates/header', $data);
        $this->load->view('games/index', $data);
        $this->load->view('templates/footer', $data);
       // echo "Hola Mundo cruel";
    }
}
