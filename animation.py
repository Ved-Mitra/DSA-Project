from manim import *
import pandas as pd
import networkx as nx
import numpy as np

class SNIEAnimation(Scene):

    def construct(self):
        # LOAD DATA
        nodes_df = pd.read_csv("nodes.csv")
        edges_df = pd.read_csv("edges.csv")
        rec_df   = pd.read_csv("recommedation.csv")
        comm_df  = pd.read_csv("communities.csv")

        nodes_df["id"] = nodes_df["id"].astype(str)
        edges_df["u"]  = edges_df["u"].astype(str)
        edges_df["v"]  = edges_df["v"].astype(str)
        rec_df["ID"]   = rec_df["ID"].astype(str)
        comm_df["ID"]  = comm_df["ID"].astype(str)
        comm_df["communityID"] = comm_df["communityID"].astype(int)

        recommended = rec_df["ID"].tolist()
        start_node  = recommended[0]

        # BUILD GRAPH
        G = nx.Graph()

        for _, row in nodes_df.iterrows():
            node_id = row["id"]
            comm_id = int(comm_df[comm_df["ID"] == node_id]["communityID"].values[0])
            G.add_node(
                node_id,
                domain=int(row["domain"]),
                community=comm_id
            )

        for _, row in edges_df.iterrows():
            G.add_edge(row["u"], row["v"])
        # COMMUNITY POSITIONING (CENTERED + SPACED PERFECTLY)

        # Spring layout for internal cluster shape
        pos2d_raw = nx.spring_layout(G, seed=42, k=1.4, iterations=300)
        pos2d = {n: np.array([xy[0], xy[1], 0]) for n, xy in pos2d_raw.items()}

        num_comms = 5

        # PERFECT centered spacing
        R = 2.8                          # radius of community circle
        angle_step = TAU / num_comms
        vertical_shift = np.array([0, -0.4, 0])   # small downward shift

        # cluster centers in a circle around ORIGIN
        cluster_centers = {
            c: np.array([
                R * np.cos(angle_step * (c - 1)),
                R * np.sin(angle_step * (c - 1)),
                0
            ]) + vertical_shift
            for c in range(1, num_comms + 1)
        }

        INTERNAL_SPACING = 3.5  # Perfect internal node spacing

        pos = {}
        for n, xy in pos2d.items():
            com = G.nodes[n]["community"]
            pos[n] = xy * INTERNAL_SPACING + cluster_centers[com]
            
        # MANIM GRAPH   
        domain_colors = {1: RED, 2: GREEN, 3: BLUE, 4: ORANGE}

        node_config = {
            n: {
                "fill_color": domain_colors[G.nodes[n]["domain"]],
                "radius": 0.12,
                "stroke_width": 2
            }
            for n in G.nodes()
        }

        edge_config = {e: {"stroke_width": 2, "stroke_color": GRAY} for e in G.edges()}

        mg = Graph(
            list(G.nodes()),
            list(G.edges()),
            layout=pos,
            vertex_config=node_config,
            edge_config=edge_config
        )

        # =====================================================
        # SCENE 1 — ORIGINAL NETWORK
        # =====================================================
        title1 = Text("SNIE — Phase 1: Original Network", font_size=36).to_edge(UP)
        self.play(Write(title1))
        self.play(Create(mg), run_time=3)

        # COMMUNITY LABELS
        community_labels = []

        for c in range(1, num_comms + 1):
            lbl = Text(f"Community {c}", font_size=28, color=YELLOW)

            # Default position (same for everyone)
            base_pos = cluster_centers[c] + np.array([0, 1.0, 0])
            # Community 1 → slightly right + slightly down
            if c == 1:
                base_pos = cluster_centers[c] + np.array([1.3, -0.2, 0])

            # Community 2 → slightly left
            if c == 2:
                base_pos = cluster_centers[c] + np.array([-0.2, 0.2, 0])

            if c == 3:
                base_pos = cluster_centers[c] + np.array([-1.1, -0.2, 0])

            # Community 5 → down + right
            if c == 5:
                base_pos = cluster_centers[c] + np.array([0.5, -0.2, 0])

            lbl.move_to(base_pos)
            community_labels.append(lbl)

        self.play(*[FadeIn(lbl) for lbl in community_labels])
        self.wait(1)

        # =====================================================
        # SCENE 2 — DASHED EDGES
        # =====================================================
        title2 = Text("SNIE — Phase 2: Suggested Bridges", font_size=36).to_edge(UP)
        self.play(Transform(title1, title2))

        dashed_edges = []
        for target in recommended:
            if target in G.nodes:
                d = DashedLine(
                    mg[start_node].get_center(),
                    mg[target].get_center(),
                    stroke_color=YELLOW,
                    dash_length=0.2,
                    stroke_width=4
                )
                dashed_edges.append(d)

        for d in dashed_edges:
            self.play(Create(d))

        self.wait(1)

        # =====================================================
        # SCENE 3 — SOLID BRIDGES
        # =====================================================
        title3 = Text("SNIE — Phase 3: Final Network After Bridges", font_size=36).to_edge(UP)
        self.play(Transform(title1, title3))

        solid_edges = []
        for target in recommended:
            if target in G.nodes:
                l = Line(
                    mg[start_node].get_center(),
                    mg[target].get_center(),
                    stroke_color=YELLOW,
                    stroke_width=5
                )
                solid_edges.append(l)

        for s in solid_edges:
            self.play(Create(s))

        self.play(*[FadeOut(d) for d in dashed_edges])
        self.wait(2)
