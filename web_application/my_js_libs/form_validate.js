$('.form-validate').each(function() {
    $(this).validate({
        errorElement: "div",
        errorClass: 'is-invalid',
        validClass: 'is-valid',
        ignore: ':hidden:not(.summernote, .checkbox-template, .form-control-custom),.note-editable.card-block',
        errorPlacement: function (error, element) {
            // Add the `invalid-feedback` class to the error element
            error.addClass("invalid-feedback");
            console.log(element);
            error.insertAfter(element);
        }
    });
});