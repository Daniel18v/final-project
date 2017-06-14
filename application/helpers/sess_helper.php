<?php
defined("BASEPATH") or exit("No direct script access allowed");

if (!function_exists('save_uri')) {

    /**
     * Keeps the uri before login in a 'redirect_back' session variable.
     *
     * @param string $ci Codeigniter object.
     */
    function save_uri($ci = '') {
        $ci->load->library('user_agent');
        $ci->session->set_userdata('redirect_back', $ci->agent->referrer());
    }
}

if (!function_exists('redirect_back')) {

    /**
     * Checks if exist a 'redirect_back' session variable and redirect user to his before uri.
     *
     * @param string $ci Codeigniter object
     */
    function redirect_back($ci = '') {
        if ($ci->session->userdata('redirect_back')) {
            $redirect_url = $ci->session->userdata('redirect_back');
            $ci->session->unset_userdata('redirect_back');
            redirect($redirect_url);
        }
    }
}


