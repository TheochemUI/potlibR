## usethis namespace: start
#' @useDynLib potlibR, .registration = TRUE
## usethis namespace: end
NULL

#' @export
cuh2pot_list <- function(df, hasFixedCol=TRUE) {
  if (hasFixedCol) {
    # If 'is_fixed' is present in the dataframe, call the C++ function with a_isFixed = TRUE
    .Call(`_potlibR_cuh2pot_list`, df, TRUE)
  } else {
    # Otherwise, call the C++ function with a_isFixed = FALSE
    warning("Called without an is_fixed column, forces are not physically relevant")
    .Call(`_potlibR_cuh2pot_list`, df, FALSE)
  }
}
