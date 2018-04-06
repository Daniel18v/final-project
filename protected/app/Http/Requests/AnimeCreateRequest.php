<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class AnimeCreateRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     *
     * @return bool
     */
    public function authorize()
    {
        return true;
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array
     */
    public function rules()
    {
        return [
            'title' => 'required|max:255',
            'logo' => 'required',
            'episodes' => 'required|numeric|min:1|max:999',
            'slug' => 'required|max:255',
            'excerpt' => 'required|max:1000',
            'year' => 'numeric|min:1970|max:2090',
            'status' => 'required',
            'active' => 'numeric|min:0|max:1'
        ];
    }
}
