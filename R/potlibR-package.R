## usethis namespace: start
#' @useDynLib potlibR, .registration = TRUE
## usethis namespace: end
NULL

#' Computes energy and forces for the given atom coordinates of a system of Cu
#' and H
#'
#' This function computes the energy and forces for the given atom coordinates,
#' using the specified parameters. The computations are performed in C++ for
#' efficiency.
#'
#' @param df A dataframe that contains the atom coordinates and types.  The
#'   dataframe should have columns 'x', 'y', 'z', 'atmNum' and optionally
#'   'is_fixed'.  @param hasFixedCol A logical that indicates if the 'is_fixed'
#'   column is present in the dataframe.  If TRUE and 'is_fixed' column is not
#'   found, it throws an error.  If FALSE and 'is_fixed' column is found, it
#'   gives a warning. Default is TRUE.
#'
#' @return A list with elements 'energy' and 'forces'. 'energy' is a single
#'   numeric value, 'forces' is a matrix of forces for each atom.
#'
#' @export
#'
#' @examples
 \dontrun{
 # Create a dataframe with 10 atoms
 dfx <- data.frame(
   x = runif(10), # x coordinates
   y = runif(10), # y coordinates
   z = runif(10), # z coordinates
   atmNum = sample(c(1, 29), 10, replace = TRUE), # Atom numbers (1 for hydrogen, 29 for copper)
   is_fixed = sample(c(TRUE, FALSE), 10, replace = TRUE) # Optional: whether each atom is fixed
 )

 # Compute energy and forces, considering 'is_fixed' column
 result <- cuh2pot_list(dfx, hasFixedCol = TRUE)

 # Compute energy and forces, ignoring 'is_fixed' column
 # This will give a warning because 'is_fixed' column is present in the dataframe
 result <- cuh2pot_list(dfx, hasFixedCol = FALSE)
 }
cuh2pot_list <- function(df, hasFixedCol = TRUE) {
  if (hasFixedCol) {
    # If 'is_fixed' should be present in the dataframe, but is not
    if (!("is_fixed" %in% names(df))) {
      stop("Error: 'is_fixed' column expected but not found in dataframe.")
    }
    .Call(`_potlibR_cuh2pot_list`, df, TRUE)
  } else {
    # If 'is_fixed' should not be present in the dataframe, but is
    if ("is_fixed" %in% names(df)) {
      warning("Warning: 'is_fixed' column found in dataframe, but 'hasFixedCol' is set to FALSE. Forces may not be physically relevant.")
    }
    .Call(`_potlibR_cuh2pot_list`, df, FALSE)
  }
}
