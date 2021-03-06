#ifndef __RAPTER_PROBLEMSETUP_H__
#define __RAPTER_PROBLEMSETUP_H__

#include <vector>
#include <map>
#include <set>

#include "qcqpcpp/optProblem.h"     // OptProblem
#include "rapter/parameters.h"      // ProblemSetupParams
#include "rapter/util/pclUtil.h"    // PclCloudPtrT

namespace rapter
{
    // predecl
    template <typename _Scalar, class _PrimitiveT> struct AbstractPrimitivePrimitiveEnergyFunctor;

    namespace problemSetup
    {
        //! \brief General problem type, most implementations require double, so it is fixed to double.
        typedef qcqpcpp::OptProblem<double> OptProblemT;

        //! \brief              Adds constraints to \p problem so, that each patch (prims[i] that have the same _PrimitiveT::TAGS::GID) has at least one member j (prims[i][j]) selected.
        //! \tparam _AssocT     Associates a primitive identified by <lid,lid1> with a variable id in the problem. Default: std::map< std::pair<int,int>, int >
        template < class _PointPrimitiveDistanceFunctor
                 , class _PrimitiveT        /* = typename _PrimitiveContainerT::value_type::value_type */
                 , class _PointPrimitiveT   /* = typename _PointContainerT::value_type */
                 , typename _Scalar
                 , class _OptProblemT
                 , class _PrimitiveContainerT
                 , class _PointContainerT
                 , class _AssocT
                 , class _WeightsT
                 >
        static inline int
        everyPatchNeedsDirectionConstraint( _OptProblemT              & problem
                                          , _PrimitiveContainerT const& prims
                                          , _PointContainerT     const& /*points*/
                                          , _AssocT              const& lids_varids
                                          , _WeightsT            const& /*weights*/
                                          , _Scalar              const  /*scale*/
                                          , bool                 const  verbose = false );

        //! \brief              Adds unary costs to problem based on point to primitive associations.
        //! \tparam _AssocT     Associates a primitive identified by <lid,lid1> with a variable id in the problem. Default: std::map< std::pair<int,int>, int >
        template < class _PointPrimitiveDistanceFunctor
                 , class _PrimitiveT        /* = typename _PrimitiveContainerT::value_type::value_type */
                 , class _PointPrimitiveT   /* = typename _PointContainerT::value_type */
                 , typename _Scalar
                 , class _OptProblemT
                 , class _PrimitiveContainerT
                 , class _PointContainerT
                 , class _AssocT
                 , class _WeightsT
                 >
        static inline int
        associationBasedDataCost( _OptProblemT              & problem
                                , _PrimitiveContainerT const& prims
                                , _PointContainerT     const& points
                                , _AssocT              const& lids_varids
                                , _WeightsT            const& weights
                                , _Scalar              const  scale
                                , _Scalar              const  freq_weight
                                , bool                 const  verbose );
    } //... namespace problemSetup

    //! \brief Class to formulate problem into an quadratic optimization problem.
    class ProblemSetup
    {
        public:
             /*! \brief                          Step 2. Reads the output from generate and sets up the optimization problem in form of sparse matrices.
              *  \tparam _PrimitiveContainerT    Concept: vector< vector< \ref rapter::LinePrimitive2 > >
              *  \tparam _PointContainerT        Concept: vector< \ref rapter::PointPrimitive >
              *  \tparam _FiniteFiniteDistFunctor Concept: \ref rapter::SpatialSqrtPrimitivePrimitiveEnergyFunctor.
              *  \param argc                     Number of CLI arguments.
              *  \param argv                     Vector of CLI arguments.
              *  \return                         Outputs EXIT_SUCCESS or the error the OptProblem implementation returns.
              *  \sa \ref problemSetup::largePatchesNeedDirectionConstraint
                                                         */
            template <
                       class _PrimitiveContainerT
                     , class _PointContainerT
                     , class _PrimitiveT /*         = typename _PrimitiveContainerT::value_type::value_type*/
                     , class _PointPrimitiveT     /*= typename _PointContainerT::value_type*/
                     , class _FiniteFiniteDistFunctor
                     >
            static inline int formulateCli( int argc, char** argv );
#if 0
            /*! \brief                          Step 2. Reads the output from generate and sets up the optimization problem in form of sparse matrices.
             *  \tparam _PrimitiveContainerT    Concept: vector< vector< \ref rapter::LinePrimitive2 > >.
             *  \tparam _PointContainerT        Concept: vector< \ref rapter::PointPrimitive >.
             *  \tparam _PrimPrimDistFunctorT   Concept: \ref rapter::SpatialSqrtPrimitivePrimitiveEnergyFunctor.
             *  \param[in,out] problem          QCQPcpp::OptProblem (\ref problemSetup::OptProblemT) typed problem to append to.
             *  \param[in] prims                Holds the input primitives to setup the optimization with.
             *  \param[in] points               Holds the input points.
             *  \param[in] constr_mode          See \ref ProblemSetupParams::CONSTR_MODE. \copybrief rapter::ProblemSetupParams::CONSTR_MODE.
             *  \param[in] data_cost_mode       See \ref ProblemSetupParams::DATA_COST_MODE.
             *  \param[in] scale                Scale of input, can be used for data cost.
             *  \param[in] weights              Weights for problemsetup, contains data, pairwise and complexity weight in this order.
             *  \param[in] primPrimDistFunctor  Functor, that calculates the distance between two primitives. Concept: \ref SqrtPrimitivePrimitiveEnergyFunctor.
             *  \param[in] patch_pop_limit      Decides, whether a patch is large or small. Used in \ref problemSetup::largePatchesNeedDirectionConstraint.
             *  \param[in] dir_id_bias          \copydoc ProblemSetupParams::dir_id_bias.
             *  \param[in] verbose              Debug messages display.
             *  \param[in] freq_weight          Multiplies the data cost by freq_weight / DIR_COUNT.
             *  \return                         Outputs EXIT_SUCCESS or the error the OptProblem implementation returns.
             *  \note                           \p points are assumed to be tagged at _PointPrimitiveT::TAGS::GID with the _PrimitiveT::TAGS::GID of the \p prims.
             *  \sa \ref problemSetup::largePatchesNeedDirectionConstraint
             */
            template < class _PointPrimitiveDistanceFunctor
                     , class _PrimitiveContainerT
                     , class _PointContainerT
                     , class _PrimPrimDistFunctorT
                     , class _PrimitiveT        = typename _PrimitiveContainerT::value_type::value_type
                     , class _PointPrimitiveT   = typename _PointContainerT::value_type
                     , typename _Scalar         = typename _PointPrimitiveT::Scalar
                     > static inline int
            formulate( problemSetup::OptProblemT                                               & problem
                     , _PrimitiveContainerT                                               const& prims
                     , _PointContainerT                                                   const& points
                     , typename ProblemSetupParams<_Scalar>::CONSTR_MODE                  const  constr_mode
                     , typename ProblemSetupParams<_Scalar>::DATA_COST_MODE               const  data_cost_mode
                     , _Scalar                                                            const  scale
                     , Eigen::Matrix<_Scalar,-1,1>                                        const& weights
                     , _PrimPrimDistFunctorT                                            * const& primPrimDistFunctor
                     , AnglesT                                                            const& angle_gens
                     , int                                                                const  patch_pop_limit
                     //, _Scalar                                                            const  dir_id_bias
                     , int                                                                const  verbose                = 0
                     , _Scalar                                                            const  freq_weight            = 0.
                     , int                                                                const  clusterMode            = 1
                     );
#endif

            /*! \brief                          Step 2. Reads the output from generate and sets up the optimization problem in form of sparse matrices.
             *  \tparam _PrimitiveContainerT    Concept: vector< vector< \ref rapter::LinePrimitive2 > >.
             *  \tparam _PointContainerT        Concept: vector< \ref rapter::PointPrimitive >.
             *  \tparam _PrimPrimDistFunctorT   Concept: \ref rapter::SpatialSqrtPrimitivePrimitiveEnergyFunctor.
             *  \param[in,out] problem          QCQPcpp::OptProblem (\ref problemSetup::OptProblemT) typed problem to append to.
             *  \param[in] prims                Holds the input primitives to setup the optimization with.
             *  \param[in] points               Holds the input points.
             *  \param[in] constr_mode          See \ref ProblemSetupParams::CONSTR_MODE. \copybrief rapter::ProblemSetupParams::CONSTR_MODE.
             *  \param[in] data_cost_mode       See \ref ProblemSetupParams::DATA_COST_MODE.
             *  \param[in] scale                Scale of input, can be used for data cost.
             *  \param[in] weights              Weights for problemsetup, contains data, pairwise and complexity weight in this order.
             *  \param[in] primPrimDistFunctor  Functor, that calculates the distance between two primitives. Concept: \ref SqrtPrimitivePrimitiveEnergyFunctor.
             *  \param[in] patch_pop_limit      Decides, whether a patch is large or small. Used in \ref problemSetup::largePatchesNeedDirectionConstraint.
             *  \param[in] dir_id_bias          \copydoc ProblemSetupParams::dir_id_bias.
             *  \param[in] verbose              Debug messages display.
             *  \param[in] freq_weight          Multiplies the data cost by freq_weight / DIR_COUNT.
             *  \return                         Outputs EXIT_SUCCESS or the error the OptProblem implementation returns.
             *  \note                           \p points are assumed to be tagged at _PointPrimitiveT::TAGS::GID with the _PrimitiveT::TAGS::GID of the \p prims.
             *  \sa \ref problemSetup::largePatchesNeedDirectionConstraint
             */
            template < class _PointPrimitiveDistanceFunctor
                     , class _PrimitiveContainerT
                     , class _PointContainerT
                     , class _PrimPrimDistFunctorT
                     , class _PrimitiveT        = typename _PrimitiveContainerT::value_type::value_type
                     , class _PointPrimitiveT   = typename _PointContainerT::value_type
                     , typename _Scalar         = typename _PointPrimitiveT::Scalar
                     > static inline int
            formulate2( problemSetup::OptProblemT                                               & problem
                     , _PrimitiveContainerT                                               const& prims
                     , _PointContainerT                                                   const& points
                     , typename ProblemSetupParams<_Scalar>::CONSTR_MODE                  const  constr_mode
                     , typename ProblemSetupParams<_Scalar>::DATA_COST_MODE               const  data_cost_mode
                     , _Scalar                                                            const  scale
                     , Eigen::Matrix<_Scalar,-1,1>                                        const& weights
                     , _PrimPrimDistFunctorT                                            * const& primPrimDistFunctor
                     , AnglesT                                                            const& angle_gens
                     , int                                                                const  patch_pop_limit
                     //, _Scalar                                                            const  dir_id_bias
                     , PclCloudPtrT                                                            & pclCloud
                     , int                                                                const  verbose                = 0
                     , _Scalar                                                            const  freq_weight            = 0.
                     , int                                                                const  clusterMode            = 1
                     , _Scalar                                                            const  collapseThreshold      = 0.07 // sqrt( 0.1 * PI / 180 ) == 0.06605545496
                     );

    }; //...class ProblemSetup
} //...namespace rapter

#endif // __RAPTER_PROBLEMSETUP_H__
